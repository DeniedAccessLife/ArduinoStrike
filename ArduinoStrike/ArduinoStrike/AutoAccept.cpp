#include "Logger.h"
#include "AutoAccept.h"

AutoAccept::AutoAccept(int activationKey) : ToggleableModule(activationKey) {}

void AutoAccept::OnToggleActive(Arduino& arduino, const Config& config)
{
    // This function is executed when the AutoAccept module becomes active.

    const int width = GetSystemMetrics(SM_CXSCREEN);
    const int height = GetSystemMetrics(SM_CYSCREEN);

    HDC hscreen = GetDC(nullptr);
    HDC hdc = CreateCompatibleDC(hscreen);
    HBITMAP hbitmap = CreateCompatibleBitmap(hscreen, width, height);

    // Select the created bitmap into the compatible device context.
    SelectObject(hdc, hbitmap);

    // Copy the screen image to the bitmap using BitBlt.
    if (!BitBlt(hdc, 0, 0, width, height, hscreen, 0, 0, SRCCOPY))
    {
        Logger::LogMessage("BitBlt failed. Exiting...");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    BITMAPINFOHEADER bitmap = { sizeof(bitmap) };
    bitmap.biWidth = width;
    bitmap.biHeight = -height;                      // Negative height to indicate top-down bitmap.
    bitmap.biPlanes = 1;
    bitmap.biBitCount = 24;                         // 24 bits for RGB color.
    bitmap.biCompression = BI_RGB;                  // No compression.

    // Calculate row size, ensuring it's a multiple of 4.
    const int row = (width * 3 + 3) & ~3;
    vector<uint8_t> pixels(row * height);

    // Retrieve the pixel data from the bitmap and store it in the 'pixels' vector.
    if (!GetDIBits(hdc, hbitmap, 0, height, pixels.data(), (BITMAPINFO*)&bitmap, DIB_RGB_COLORS))
    {
        Logger::LogMessage("GetDIBits failed. Exiting...");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    // Calculate the click position on the screen based on the target color.
    POINT position = CalculateClickPosition(pixels, width, height, row);
    Logger::LogMessage("Calculated click position: (" + to_string(position.x) + ", " + to_string(position.y) + ")");

    // Check if the calculated position is invalid (target not found).
    if (position.x == -1 || position.y == -1)
    {
        Logger::LogMessage("No target found on screen.");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    // Get the current cursor position.
    POINT current;
    if (!GetCursorPos(&current))
    {
        Logger::LogMessage("Failed to get cursor position.");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    Logger::LogMessage("Current cursor position: (" + to_string(current.x) + ", " + to_string(current.y) + ")");

    // Calculate the distance between the current cursor position and the target click position.
    const double distance = CalculateDistance(current, position);
    Logger::LogMessage("Cursor distance to target: " + to_string(distance));

    // If the cursor is within the specified range, send a click event.
    if (distance <= 5.0)
    {
        arduino.WriteMessage("MOUSE_LEFT_CLICK");
        Logger::LogMessage("Target is close enough, clicking accept button.");
    }
    else
    {
        // Calculate a step size to move the cursor closer to the target.
        const double step = min(distance / 4.0, 10.0);

        // Calculate the x and y components of the movement vector.
        const int moveX = static_cast<int>((position.x - current.x) / distance * step);
        const int moveY = static_cast<int>((position.y - current.y) / distance * step);

        Logger::LogMessage("Moving cursor by: (" + to_string(moveX) + ", " + to_string(moveY) + ")");

        // Send a message to the Arduino to move the mouse.
        arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(moveX) + "," + to_string(moveY) + ",10");
    }

    // Clean up resources.
    DeleteObject(hbitmap);
    DeleteDC(hdc);
    ReleaseDC(nullptr, hscreen);
}

double AutoAccept::CalculateDistance(const POINT& a, const POINT& b) const
{
    // Calculates the Euclidean distance between two points.
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

bool AutoAccept::IsTargetColor(uint8_t r, uint8_t g, uint8_t b) const
{
    // Checks if the given RGB color is within the defined target color range.

    // Color ranges of the CS2 match acceptance button
    uint8_t min_r = 54, max_r = 59;
    uint8_t min_g = 183, max_g = 198;
    uint8_t min_b = 82, max_b = 90;

    // Checking if the color is within the acceptable range
    return (r >= min_r && r <= max_r) && (g >= min_g && g <= max_g) && (b >= min_b && b <= max_b);
}

POINT AutoAccept::CalculateClickPosition(const vector<uint8_t>& pixels, int width, int height, int rowSize) const
{
    // Calculates the click position based on the target color within the captured screen image.

    bool found = false;
    POINT result = { -1, -1 };
    const int scanMargin = 100;

    int max_x = 0, max_y = 0;
    int min_x = width, min_y = height;

    const int y_start = max(0, height / 2 - scanMargin);
    const int y_end = min(height, height / 2 + scanMargin);
    const int x_start = max(0, width / 2 - scanMargin);
    const int x_end = min(width, width / 2 + scanMargin);

    // Iterate through the pixels within the scan area.
    for (int y = y_start; y < y_end; y++)
    {
        for (int x = x_start; x < x_end; x++)
        {
            const size_t index = y * rowSize + x * 3;

            if (index + 2 >= pixels.size())
            {
                // Skip if the index is out of bounds.
                continue;
            }

            const uint8_t r = pixels[index + 2];    // Red component.
            const uint8_t g = pixels[index + 1];    // Green component.
            const uint8_t b = pixels[index];        // Blue component.

            // Check if the current pixel's color matches the target color.
            if (IsTargetColor(r, g, b))
            {
                found = true;

                min_x = min(min_x, x);
                max_x = max(max_x, x);

                min_y = min(min_y, y);
                max_y = max(max_y, y);
            }
        }
    }

    if (found)
    {
        // Calculate the center of the found target region.
        result.x = (min_x + max_x) / 2;
        result.y = (min_y + max_y) / 2;
    }

    return result;
}