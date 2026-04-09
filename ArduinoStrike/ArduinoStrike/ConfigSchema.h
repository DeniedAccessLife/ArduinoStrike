#pragma once

#include "pch.h"
#include "json.hpp"
#include "Weapons.h"

#include <array>
#include <string>

using json = nlohmann::json;

namespace ConfigSchema
{
    struct WeaponInfo
    {
        Weapon weapon;
        const char* id;
        const char* display;
    };

    extern const std::array<WeaponInfo, 9> WEAPONS;

    const WeaponInfo* FindWeaponInfo(Weapon weapon) noexcept;
    const WeaponInfo* FindWeaponInfo(const std::string& id) noexcept;

    extern const std::array<const char*, 12> REQUIRED_KEYS;

    struct TypeCheck
    {
        const char* key;
        json::value_t type;
    };

    extern const std::array<TypeCheck, 3> TYPE_CHECKS;

    struct RangeCheck
    {
        const char* key;
        double min;
        double max;
    };

    extern const std::array<RangeCheck, 8> RANGE_CHECKS;
}


