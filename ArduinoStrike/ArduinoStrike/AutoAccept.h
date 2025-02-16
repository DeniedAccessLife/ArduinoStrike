#pragma once

#include "ToggleableModule.h"

class AutoAccept : public ToggleableModule
{
public:
    AutoAccept(int activationKey);

protected:
    void OnToggleActive(Arduino& arduino, const Config& config) override;

private:
    bool IsTargetColor(uint8_t r, uint8_t g, uint8_t b) const;
    double CalculateDistance(const POINT& a, const POINT& b) const;
    POINT CalculateClickPosition(const vector<uint8_t>& pixels, int screenWidth, int screenHeight, int rowSize) const;
};