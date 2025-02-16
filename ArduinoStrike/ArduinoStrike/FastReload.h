#pragma once

#include "Arduino.h"
#include "Weapons.h"
#include "SystemModule.h"

class FastReload : public SystemModule
{
public:
    FastReload();

    void SetCurrentWeapon(Weapon weapon);

    void Execute(Arduino& arduino, const Config& config) override;

private:
    Weapon currentWeapon = OFF;
    unordered_map<Weapon, int> times;
};