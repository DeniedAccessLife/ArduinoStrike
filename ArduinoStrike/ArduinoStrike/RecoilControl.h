#pragma once

#include "Weapons.h"
#include "ModuleManager.h"
#include "AutomaticModule.h"

class RecoilControl : public AutomaticModule
{
public:
    RecoilControl(ModuleManager& manager);

    void Execute(Arduino& arduino, const Config& config) override;

private:
    Weapon weapon;
    ModuleManager& manager;

    double CalculateModifier(const Config& config, double obs);
    bool ValidateWeaponData(const WeaponData& data);
    void ProcessRecoilData(Arduino& arduino, const Config& config, const WeaponData& data);
};