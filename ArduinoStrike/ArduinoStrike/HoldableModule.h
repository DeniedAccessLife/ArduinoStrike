#pragma once

#include "Module.h"
#include "Weapons.h"

class HoldableModule : public Module
{
public:
    explicit HoldableModule(int activationKey) : activationKey(activationKey) {}

    void Process(Arduino& arduino, const Config& config) override
    {
        if (IsKeyHolded(activationKey))
        {
            OnKeyHold(arduino, config);
        }
    }

protected:
    int GetKey() const { return activationKey; }

    virtual void OnKeyHold(Arduino& arduino, const Config& config) = 0;

private:
    int activationKey;
};