#pragma once

#include "Arduino.h"
#include "Weapons.h"

class ColorBot
{
public:
    ColorBot(int threshold, int key);
    void Process(const Arduino& arduino) const;

private:
    char key;
    int threshold;

    COLORREF GetPixelColor(int x, int y) const;
    bool IsColorDifferent(COLORREF color1, COLORREF color2, int threshold) const;
};