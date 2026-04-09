#include "pch.h"
#include "Weapons.h"
#include "Config.h"

WeaponData GetWeaponData(Weapon weapon, double modifier, const Config& config)
{
    WeaponData baseData = config.GetWeaponRecoilData(weapon);
    
    if (baseData.x.empty() && baseData.y.empty() && baseData.delay.empty())
    {
        return {};
    }
    
    WeaponData result;
    result.delay = baseData.delay;
    
    for (size_t i = 0; i < baseData.x.size(); i++)
    {
        result.x.push_back(baseData.x[i] * modifier);
    }
    
    for (size_t i = 0; i < baseData.y.size(); i++)
    {
        result.y.push_back(baseData.y[i] * modifier);
    }
    
    return result;
}

Weapon GetWeaponState(Weapon weapon, const Config& config)
{
    const std::vector<Weapon> weapons = { OFF, UMP, M4A1, M4A4, AK47, GALIL, FAMAS, AUG, SG };
    
    for (Weapon w : weapons)
    {
        if (IsKeyPressed(config.GetWeaponKey(w)) & 1)
        {
            return w;
        }
    }
    
    return weapon;
}

