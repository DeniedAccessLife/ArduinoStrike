#include "ConfigSchema.h"

namespace ConfigSchema
{
    // -------- Weapon metadata --------

    const std::array<WeaponInfo, 9> WEAPONS = { {
        { OFF,   "OFF",   "OFF" },
        { UMP,   "UMP",   "UMP-45" },
        { M4A1,  "M4A1",  "M4A1-S" },
        { M4A4,  "M4A4",  "M4A4" },
        { AK47,  "AK47",  "AK-47" },
        { GALIL, "GALIL", "Galil AR" },
        { FAMAS, "FAMAS", "FAMAS" },
        { AUG,   "AUG",   "AUG" },
        { SG,    "SG",    "SG-553" }
    } };

    const WeaponInfo* FindWeaponInfo(Weapon weapon) noexcept
    {
        for (const auto& info : WEAPONS)
        {
            if (info.weapon == weapon)
            {
                return &info;
            }
        }
        return nullptr;
    }

    const WeaponInfo* FindWeaponInfo(const std::string& id) noexcept
    {
        for (const auto& info : WEAPONS)
        {
            if (id == info.id)
            {
                return &info;
            }
        }
        return nullptr;
    }

    // -------- Config schema metadata --------

    const std::array<const char*, 12> REQUIRED_KEYS = { {
        "weaponKeys",
        "bhop",
        "rapidFire",
        "fastReload",
        "sensitivity",
        "zoomSensitivity",
        "colorThreshold",
        "confirmationKey",
        "colorBotKey",
        "autoAcceptKey",
        "turnAroundKey",
        "turnAroundDistance"
    } };

    const std::array<TypeCheck, 3> TYPE_CHECKS = { {
        { "bhop",        json::value_t::boolean },
        { "rapidFire",   json::value_t::boolean },
        { "fastReload",  json::value_t::boolean }
    } };

    const std::array<RangeCheck, 8> RANGE_CHECKS = { {
        { "sensitivity",         1.0,   8.0 },
        { "zoomSensitivity",     0.01,  3.0 },
        { "colorThreshold",      0,     20 },
        { "confirmationKey",     0,     0xFE },
        { "colorBotKey",         0,     0xFE },
        { "autoAcceptKey",       0,     0xFE },
        { "turnAroundKey",       0,     0xFE },
        { "turnAroundDistance",  100,   10000 },
    } };
}