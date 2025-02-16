#pragma once

#include "HoldableModule.h"

class Bhop : public HoldableModule
{
public:
    Bhop(int activationKey);

protected:
    void OnKeyHold(Arduino& arduino, const Config& config) override;
};