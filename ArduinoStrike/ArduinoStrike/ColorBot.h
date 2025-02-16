#pragma once

#include "HoldableModule.h"

class ColorBot : public HoldableModule
{
public:
    ColorBot(int threshold, int activationKey);

protected:
    void OnKeyHold(Arduino& arduino, const Config& config) override;

private:
    int threshold;

    COLORREF GetPixelColor(int x, int y) const;
    bool IsColorDifferent(COLORREF color1, COLORREF color2, int threshold) const;
};