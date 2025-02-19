#include "FastReload.h"
#include "Logger.h"

FastReload::FastReload() : times
{
    { UMP, 1600 },
    { M4A1, 1495 },
    { M4A4, 1450 },
    { AK47, 1200 },
    { GALIL, 1200 },
    { FAMAS, 1700 },
    { SG, 1200 }
} {}

void FastReload::SetCurrentWeapon(Weapon weapon)
{
    currentWeapon = weapon;
}

void FastReload::Execute(Arduino& arduino, const Config& config)
{
    auto iterator = times.find(currentWeapon);

    if (config.GetFastReload() && iterator != times.end())
    {
        sleep_for(milliseconds(iterator->second));
        arduino.WriteMessage("KEYBOARD_PRESS_Q");
    }
}