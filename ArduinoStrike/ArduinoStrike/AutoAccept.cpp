#include "Logger.h"
#include "AutoAccept.h"
#include "MouseHumanizer.h"

AutoAccept::AutoAccept(int activationKey) : ToggleableModule(activationKey) {}

void AutoAccept::OnToggleActive(Arduino& arduino, const Config& config)
{
    // Retrieve the full screen resolution to allocate a correctly-sized bitmap.
    const int width = GetSystemMetrics(SM_CXSCREEN);
    const int height = GetSystemMetrics(SM_CYSCREEN);

    // Acquire a device context for the entire screen.
    HDC hscreen = GetDC(nullptr);

    // Create an off-screen DC compatible with the screen DC,
    // so we can draw into it without affecting the display.
    HDC hdc = CreateCompatibleDC(hscreen);

    // Allocate a bitmap large enough to hold one full screenshot.
    HBITMAP hbitmap = CreateCompatibleBitmap(hscreen, width, height);

    // Attach the bitmap to the off-screen DC so subsequent drawing targets it.
    SelectObject(hdc, hbitmap);

    // Copy the current screen content into our off-screen bitmap (pixel-perfect copy).
    if (!BitBlt(hdc, 0, 0, width, height, hscreen, 0, 0, SRCCOPY))
    {
        Logger::LogMessage("BitBlt failed.");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    // Describe the pixel format we want: 24-bit RGB, top-down row order.
    // biHeight is negative to indicate a top-down DIB (row 0 = top of screen).
    BITMAPINFOHEADER bitmap = { sizeof(bitmap) };
    bitmap.biWidth = width;
    bitmap.biHeight = -height;
    bitmap.biPlanes = 1;
    bitmap.biBitCount = 24;      // 3 bytes per pixel: B, G, R
    bitmap.biCompression = BI_RGB;  // no compression

    // Row stride must be a multiple of 4 bytes (Win32 DIB requirement).
    // Each pixel is 3 bytes wide, so: round up (width * 3) to the next multiple of 4.
    const int row = (width * 3 + 3) & ~3;

    // Raw pixel buffer: one byte per channel, laid out as rows of `row` bytes.
    vector<uint8_t> pixels(row * height);

    // Extract the raw pixel data from the bitmap into our buffer.
    if (!GetDIBits(hdc, hbitmap, 0, height, pixels.data(), (BITMAPINFO*)&bitmap, DIB_RGB_COLORS))
    {
        Logger::LogMessage("GetDIBits failed.");
        DeleteObject(hbitmap);
        DeleteDC(hdc);
        ReleaseDC(nullptr, hscreen);
        return;
    }

    // GDI resources are no longer needed — release them before the heavy CPU work.
    DeleteObject(hbitmap);
    DeleteDC(hdc);
    ReleaseDC(nullptr, hscreen);

    // Scan the pixel buffer for the CS2 accept-button color and return its center.
    // Returns {-1, -1} when the button is not visible on screen.
    POINT position = CalculateClickPosition(pixels, width, height, row);

    if (position.x == -1 || position.y == -1)
        return; // Button not found — nothing to do this tick.

    // Sample the real cursor position at this exact moment.
    POINT current;
    if (!GetCursorPos(&current))
        return;

    // Straight-line distance (pixels) between the cursor and the button center.
    const double distance = CalculateDistance(current, position);
    Logger::LogMessage("Cursor distance to target: " + to_string(distance));

    // Close enough — fire a click, then turn the module off so it does not keep
    // searching after the button disappears (match accepted → screen changes).
    if (distance <= 15.0)
    {
        arduino.WriteMessage("MOUSE_LEFT_CLICK");
        Logger::LogMessage("Clicking accept button.");
        return;
    }

    // ── Smooth incremental approach ──────────────────────────────────────────
    //
    // Strategy: each OnToggleActive call moves the cursor ~1/3 of the remaining
    // distance, split into MICRO_STEPS tiny sub-moves sent back-to-back.
    // The next call re-reads the real cursor position and recomputes the direction,
    // so accumulated rounding errors self-correct automatically.
    //
    // Why not move the full distance at once?
    //   Arduino HID units ≠ screen pixels (depends on Windows pointer speed),
    //   so large single moves overshoot and cause oscillation between corners.

    thread_local std::mt19937 rng{ std::random_device{}() };

    // ±1 pixel jitter added to each sub-step — makes the path look organic,
    // not perfectly straight.
    std::uniform_int_distribution<int> noiseDist(-1, 1);

    // Randomised per-step delay keeps the speed variation human-like.
    std::uniform_int_distribution<int> delayDist(5, 9);

    // Total pixels to cover this call: at most 80px so the cursor never jumps
    // visibly even on the first call from a far corner.
    const double chunkSize = std::min(distance / 3.0, 80.0);

    // Number of sub-moves per call. More steps = smoother appearance.
    constexpr int MICRO_STEPS = 6;

    // Pixels per individual sub-move (floating point — rounded when sent).
    const double perStep = chunkSize / MICRO_STEPS;

    // Unit direction vector pointing from the current cursor toward the button.
    const double dirX = (position.x - current.x) / distance;
    const double dirY = (position.y - current.y) / distance;

    for (int i = 0; i < MICRO_STEPS; i++)
    {
        // Project the step distance onto each axis and add a small random jitter.
        const int mx = static_cast<int>(dirX * perStep) + noiseDist(rng);
        const int my = static_cast<int>(dirY * perStep) + noiseDist(rng);

        // Skip degenerate zero-move steps to avoid flooding the serial port.
        if (mx == 0 && my == 0)
            continue;

        // Randomise the delay so consecutive steps vary slightly in speed.
        const int delay = delayDist(rng);

        // Send a relative mouse move to the Arduino.
        // Protocol: "MOUSE_LEFT_HOLDED:dx,dy,duration_ms"
        arduino.WriteMessage(
            "MOUSE_LEFT_HOLDED:" + to_string(mx) + "," + to_string(my) + "," + to_string(delay)
        );

        // Block for exactly as long as the Arduino will hold the move,
        // so the next sub-step starts after the hardware has executed this one.
        sleep_for(milliseconds(delay));
    }
}

double AutoAccept::CalculateDistance(const POINT& a, const POINT& b) const
{
    // Standard 2-D Euclidean distance between two screen points.
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

bool AutoAccept::IsTargetColor(uint8_t r, uint8_t g, uint8_t b) const
{
    // Returns true when the given RGB triplet falls within the color range
    // of the CS2 "Accept Match" button (sampled empirically).
    //
    // Tight ranges are intentional — the button has a distinctive green tint
    // that rarely appears elsewhere on the CS2 UI.
    const uint8_t min_r = 54, max_r = 59;
    const uint8_t min_g = 183, max_g = 198;
    const uint8_t min_b = 82, max_b = 90;

    return (r >= min_r && r <= max_r)
        && (g >= min_g && g <= max_g)
        && (b >= min_b && b <= max_b);
}

POINT AutoAccept::CalculateClickPosition(
    const vector<uint8_t>& pixels,
    int width, int height, int rowSize) const
{
    POINT result = { -1, -1 }; // Default: button not found.
    bool  found = false;

    // Bounding box of all matching pixels found so far.
    // We accumulate min/max to find the button's center rather than its edge.
    int min_x = width, max_x = 0;
    int min_y = height, max_y = 0;

    // Only scan the middle 200×200 pixel region of the screen.
    // The CS2 accept button always appears near the center, so searching the
    // full screen would waste CPU and risk false positives on other UI elements.
    const int scanMargin = 100;
    const int y_start = max(0, height / 2 - scanMargin);
    const int y_end = min(height, height / 2 + scanMargin);
    const int x_start = max(0, width / 2 - scanMargin);
    const int x_end = min(width, width / 2 + scanMargin);

    for (int y = y_start; y < y_end; y++)
    {
        for (int x = x_start; x < x_end; x++)
        {
            // Byte offset into the pixel buffer.
            // Each row is `rowSize` bytes; each pixel occupies 3 bytes in BGR order.
            const size_t index = y * rowSize + x * 3;

            // Guard against the buffer overrun that could occur on the last row
            // when rowSize padding pushes the index past the allocated size.
            if (index + 2 >= pixels.size())
                continue;

            // Win32 DIBs store channels in BGR order, not RGB.
            const uint8_t r = pixels[index + 2];
            const uint8_t g = pixels[index + 1];
            const uint8_t b = pixels[index];

            if (IsTargetColor(r, g, b))
            {
                found = true;

                // Expand the bounding box to include this pixel.
                min_x = min(min_x, x); max_x = max(max_x, x);
                min_y = min(min_y, y); max_y = max(max_y, y);
            }
        }
    }

    if (found)
    {
        // Use the geometric center of the bounding box as the click target.
        // This is more robust than taking the first matching pixel — the button
        // is a filled rectangle, so the center is always a valid click point.
        result.x = (min_x + max_x) / 2;
        result.y = (min_y + max_y) / 2;
    }

    return result;
}