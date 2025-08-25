#pragma once
#include "Config.h"

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
    vector<double> x;
    vector<double> y;
    vector<int> delay;
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

inline WeaponData GetWeaponData(Weapon weapon, double modifier)
{
    switch (weapon)
    {
        case UMP:
            return {
                { -1 * modifier, -4 * modifier, -2 * modifier, -4 * modifier, -9 * modifier, -3 * modifier, 11 * modifier, -4 * modifier, 9 * modifier, 18 * modifier, 15 * modifier, -1 * modifier, 5 * modifier, 0 * modifier, 9 * modifier, 5 * modifier, -12 * modifier, -19 * modifier, -1 * modifier, 15 * modifier, 17 * modifier, -6 * modifier, -20 * modifier, -3 * modifier, -3 * modifier },
                { 6 * modifier, 8 * modifier, 18 * modifier, 23 * modifier, 23 * modifier, 26 * modifier, 17 * modifier, 12 * modifier, 13 * modifier, 8 * modifier, 5 * modifier, 3 * modifier, 6 * modifier, 6 * modifier, -3 * modifier, -1 * modifier, 4 * modifier, 1 * modifier, -2 * modifier, -5 * modifier, -2 * modifier, 3 * modifier, -2 * modifier, -1 * modifier, -1 * modifier },
                { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 85, 90, 90, 85, 90, 90, 90, 90 }
            };

        case M4A1:
            return {
                { 1 * modifier, 0 * modifier, -4 * modifier, 4 * modifier, -6 * modifier, -4 * modifier, 14 * modifier, 8 * modifier, 18 * modifier, -4 * modifier, -14 * modifier, -25 * modifier, -19 * modifier, -22 * modifier, 1 * modifier, 8 * modifier, -9 * modifier, -13 * modifier, 3 * modifier, 1 * modifier },
                { 6 * modifier, 4 * modifier, 14 * modifier, 18 * modifier, 21 * modifier, 24 * modifier, 14 * modifier, 12 * modifier, 5 * modifier, 10 * modifier, 5 * modifier, -3 * modifier, 0 * modifier, -3 * modifier, 3 * modifier, 3 * modifier, 1 * modifier, -2 * modifier, 2 * modifier, 1 * modifier },
                { 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 215 }
            };

        case M4A4:
            return {
                { 0, 2 * modifier, 0 * modifier, -6 * modifier, 7 * modifier, -9 * modifier, -5 * modifier, 16 * modifier, 11 * modifier, 22 * modifier, -4 * modifier, -18 * modifier, -30 * modifier, -24 * modifier, -25 * modifier, 0 * modifier, 8 * modifier, -11 * modifier, -13 * modifier, 2 * modifier, 33 * modifier, 10 * modifier, 27 * modifier, 10 * modifier, 11 * modifier, -12 * modifier, 6 * modifier, 4 * modifier, 3 * modifier, 4 * modifier },
                { 0, 7 * modifier, 9 * modifier, 16 * modifier, 21 * modifier, 23 * modifier, 27 * modifier, 15 * modifier, 13 * modifier, 5 * modifier, 11 * modifier, 6 * modifier, -4 * modifier, 0 * modifier, -6 * modifier, 4 * modifier, 4 * modifier, 1 * modifier, -2 * modifier, 2 * modifier, -1 * modifier, 6 * modifier, 3 * modifier, 2 * modifier, 0 * modifier, 0 * modifier, 5 * modifier, 5 * modifier, 1 * modifier, -1 * modifier },
                { 15, 88, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 215 }
            };

        case AK47:
            return {
                { -1 * modifier, 0 * modifier, 1 * modifier, 4 * modifier, 8 * modifier, 7 * modifier, -1 * modifier,-12 * modifier,-22 * modifier,-19 * modifier,-13 * modifier,-12 * modifier,-8 * modifier,3 * modifier, 17 * modifier,23 * modifier,24 * modifier,21 * modifier,15 * modifier,3 * modifier, -1 * modifier,-2 * modifier,1 * modifier, 3 * modifier, -2 * modifier,-16 * modifier,370 * modifier,550 * modifier,600 * modifier },
                {15 * modifier, 20 * modifier, 25 * modifier, 30 * modifier, 32 * modifier, 29 * modifier, 25 * modifier, 11 * modifier, 5 * modifier, 3 * modifier, 3 * modifier, 2 * modifier, 2 * modifier, 2 * modifier, 4 * modifier, 5 * modifier, 4 * modifier, 1 * modifier, -3 * modifier, -1 * modifier, 2 * modifier, 5 * modifier, 5 * modifier, 2 * modifier, 1 * modifier, -2 * modifier, -6 * modifier, -9 * modifier, -8 * modifier},
                { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 }
            };

        case GALIL:
            return {
                { 4 * modifier, -2 * modifier, 6 * modifier, 12 * modifier, -1 * modifier, 2 * modifier, 6 * modifier, 11 * modifier, -4 * modifier, -22 * modifier, -30 * modifier, -29 * modifier, -9 * modifier, -12 * modifier, -7 * modifier, 0 * modifier, 4 * modifier, 25 * modifier, 14 * modifier, 25 * modifier, 31 * modifier, 6 * modifier, -12 * modifier, 13 * modifier, 10 * modifier, 16 * modifier, -9 * modifier, -32 * modifier, -24 * modifier, -15 * modifier, 6 * modifier, -14 * modifier, -24 * modifier, -13 * modifier, -13 * modifier },
                { 4 * modifier, 5 * modifier, 10 * modifier, 15 * modifier, 21 * modifier, 24 * modifier, 16 * modifier, 10 * modifier, 14 * modifier, 8 * modifier, -3 * modifier, -13 * modifier, 8 * modifier, 2 * modifier, 1 * modifier, 1 * modifier, 7 * modifier, 7 * modifier, 4 * modifier, -3 * modifier, -9 * modifier, 3 * modifier, 3 * modifier, -1 * modifier, -1 * modifier, -4 * modifier, 5 * modifier, -5 * modifier, -3 * modifier, 5 * modifier, 8 * modifier, -3 * modifier, -14 * modifier, -1 * modifier, -1 * modifier },
                { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 50, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 }
            };

        case FAMAS:
            return {
                { -4 * modifier, 1 * modifier, -6 * modifier, -1 * modifier, 0 * modifier, 14 * modifier, 16 * modifier, -6 * modifier, -20 * modifier, -16 * modifier, -13 * modifier, 4 * modifier, 23 * modifier, 12 * modifier, 20 * modifier, 5 * modifier, 15 * modifier, 3 * modifier, -4 * modifier, -25 * modifier, -3 * modifier, 11 * modifier, 15 * modifier, 15 * modifier, 15 * modifier },
                { 5 * modifier, 4 * modifier, 10 * modifier, 17 * modifier, 20 * modifier, 18 * modifier, 12 * modifier, 12 * modifier, 8 * modifier, 5 * modifier, 2 * modifier, 5 * modifier, 4 * modifier, 6 * modifier, -3 * modifier, 0 * modifier, 0 * modifier, 5 * modifier, 3 * modifier, -1 * modifier, 2 * modifier, 0 * modifier, -7 * modifier, -10 * modifier, -10 * modifier },
                { 30, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 87, 88, 88, 88, 88, 88, 80, 88, 80, 84, 80, 88, 215 }
            };

        case AUG:
            return {
                { 5 * modifier, 0 * modifier, -5 * modifier, -7 * modifier, 15 * modifier, 6 * modifier, 10 * modifier, 12 * modifier, 14 * modifier, -18 * modifier, -20 * modifier, 9 * modifier, 10 * modifier, -22 * modifier, -10 * modifier, -20 * modifier, -8 * modifier, -10 * modifier, -9 * modifier, 15 * modifier, 18 * modifier, 15 * modifier, -5 * modifier, 15 * modifier, 15 * modifier, 15 * modifier, -16 * modifier, -19 * modifier, -14 * modifier, -8 * modifier },
                { 6 * modifier, 13 * modifier, 22 * modifier, 26 * modifier, 7 * modifier, 30 * modifier, 21 * modifier, 15 * modifier, 13 * modifier, 11 * modifier, 6 * modifier, 0 * modifier, 6 * modifier, 5 * modifier, -11 * modifier, -13 * modifier, 6 * modifier, 5 * modifier, 0 * modifier, 1 * modifier, 3 * modifier, 6 * modifier, 1 * modifier, -3 * modifier, -11 * modifier, 0 * modifier, 6 * modifier, 3 * modifier, 1 * modifier, 1 * modifier },
                { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 }
            };

        case SG:
            return {
                { -4 * modifier, -13 * modifier, -9 * modifier, -6 * modifier, -8 * modifier, -8 * modifier, -6 * modifier, -5 * modifier, 7 * modifier, 6 * modifier, -8 * modifier, -15 * modifier, -5 * modifier, -6 * modifier, -8 * modifier, -2 * modifier, -18 * modifier, -20 * modifier, -10 * modifier, 19 * modifier, 20 * modifier, 15 * modifier, 20 * modifier, 15 * modifier, 20 * modifier, 20 * modifier, 20 * modifier, 20 * modifier, 16 * modifier, 6 * modifier },
                { 9 * modifier, 15 * modifier, 25 * modifier, 29 * modifier, 31 * modifier, 36 * modifier, 8 * modifier, 7 * modifier, 9 * modifier, 9 * modifier, 12 * modifier, 8 * modifier, 5 * modifier, 5 * modifier, 6 * modifier, 6 * modifier, -6 * modifier, -13 * modifier, -9 * modifier, -2 * modifier, 3 * modifier, -5 * modifier, -1 * modifier, -1 * modifier, -1 * modifier, -1 * modifier, 9 * modifier, 6 * modifier, 5 * modifier, 5 * modifier },
                { 99, 99, 99, 99, 99, 99, 45, 45, 45, 45, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 45, 45, 45, 45, 99, 99, 99, 215 }
            };

        default:
            return {};
    }
}

inline Weapon GetWeaponState(Weapon weapon, const Config& config)
{
    if (IsKeyPressed(config.GetOFFKey()) & 1) return OFF;
    if (IsKeyPressed(config.GetUMPKey()) & 1) return UMP;
    if (IsKeyPressed(config.GetM4A1Key()) & 1) return M4A1;
    if (IsKeyPressed(config.GetM4A4Key()) & 1) return M4A4;
    if (IsKeyPressed(config.GetAK47Key()) & 1) return AK47;
    if (IsKeyPressed(config.GetGALILKey()) & 1) return GALIL;
    if (IsKeyPressed(config.GetFAMASKey()) & 1) return FAMAS;
    if (IsKeyPressed(config.GetAUGKey()) & 1) return AUG;
    if (IsKeyPressed(config.GetSGKey()) & 1) return SG;
    return weapon;
}