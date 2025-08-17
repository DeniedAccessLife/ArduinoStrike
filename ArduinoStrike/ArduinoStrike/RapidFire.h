#pragma once

#include "HoldableModule.h"

class RapidFire : public HoldableModule
{
public:
    RapidFire(int activationKey);

protected:
    void OnKeyHold(Arduino& arduino, const Config& config) override;
};