#include "ColorBot.h"

ColorBot::ColorBot(int threshold, int key) : threshold(threshold), key(key)
{
    SetProcessDPIAware();
}

COLORREF ColorBot::GetPixelColor(int x, int y) const
{
    HDC hdc = GetDC(NULL);
    COLORREF color = GetPixel(hdc, x, y);
    ReleaseDC(NULL, hdc);

    return color;
}

bool ColorBot::IsColorDifferent(COLORREF color1, COLORREF color2, int threshold) const
{
    int red1 = GetRValue(color1), green1 = GetGValue(color1), blue1 = GetBValue(color1);
    int red2 = GetRValue(color2), green2 = GetGValue(color2), blue2 = GetBValue(color2);

    return (abs(red1 - red2) > threshold || abs(green1 - green2) > threshold || abs(blue1 - blue2) > threshold);
}

void ColorBot::Process(const Arduino& arduino) const
{
    POINT cursor;
    GetCursorPos(&cursor);

    COLORREF initial = GetPixelColor(cursor.x + 2, cursor.y + 2);

    while (IsKeyHolded(key))
    {
        GetCursorPos(&cursor);
        COLORREF current = GetPixelColor(cursor.x + 2, cursor.y + 2);

        if (IsColorDifferent(initial, current, threshold))
        {
            arduino.WriteMessage("MOUSE_LEFT_CLICK");
        }

        sleep_for(milliseconds(100));
    }
}