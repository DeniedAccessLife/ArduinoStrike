#pragma once

#include <vector>
#include <Windows.h>

class Config;

enum Weapon
{
    OFF,
    UMP,
    M4A1,
    M4A4,
    AK47,
    GALIL,
    FAMAS,
    AUG,
    SG
};

struct WeaponData
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<int> delay;
};

inline bool IsKeyPressed(int key)
{
    return (GetAsyncKeyState(key) & 1) != 0;
}

inline bool IsKeyHolded(int key)
{
    if (key == VK_LBUTTON)
    {
        return (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
    }
    
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

WeaponData GetWeaponData(Weapon weapon, double modifier, const Config& config);
Weapon GetWeaponState(Weapon weapon, const Config& config);