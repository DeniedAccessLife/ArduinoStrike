#pragma once

#include "Module.h"
#include "Weapons.h"

class ToggleableModule : public Module
{
public:
    explicit ToggleableModule(int activationKey) : activationKey(activationKey), isActive(false) {}

    void Process(Arduino& arduino, const Config& config) override
    {
        if (IsKeyPressed(activationKey))
        {
            isActive = !isActive;
            Beep(isActive ? 700 : 250, 200);
        }

        if (isActive)
        {
            OnToggleActive(arduino, config);
        }
    }

protected:
    int GetKey() const { return activationKey; }

    virtual void OnToggleActive(Arduino& arduino, const Config& config) = 0;

private:
    bool isActive;
    int activationKey;
};