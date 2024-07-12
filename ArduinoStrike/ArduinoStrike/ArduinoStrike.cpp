#include "Utils.h"
#include "Arduino.h"
#include "Weapons.h"
#include "AsciiArt.h"

Weapon weapon = OFF;

static void HandleWeaponFire(Arduino& arduino, const vector<double>& x, const vector<double>& y, const vector<int>& delay)
{
    for (size_t i = 0; i < x.size(); i++)
    {
        if (IsKeyHolded(VK_LBUTTON))
        {
            sleep_for(milliseconds(delay[i]));

            if (IsKeyHolded(VK_LBUTTON))
            {
                arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(x[i]) + "," + to_string(y[i]) + "," + to_string(delay[i]));
            }
        }
    }
}

static void ProcessKeyEvents(Arduino& arduino, Config& config)
{
    if (IsKeyHolded(VK_SPACE) && config.bhop != 0)
    {
        arduino.WriteMessage("SPACE_BUTTON_HOLDED");
    }

    if (IsKeyHolded(VK_MBUTTON) && config.rapidFire != 0)
    {
        arduino.WriteMessage("MOUSE_MIDDLE_HOLDED");
    }
}

int main()
{
    Utils::Install();

    Config config;
    Utils::LoadConfig(config);

    Utils::PrintAscii(ASCII_INTRO);

    Arduino arduino("Arduino Leonardo");

    Utils::PrintAscii(ASCII_OUTRO);
    Utils::PrintHotkeys(ASCII_HOTKEYS);

    while (true)
    {
        weapon = GetWeaponState(weapon);
        string message = arduino.ReceiveMessage('\n');

        if (message.rfind("ARDUINO_INITIATED", 0) != 0)
        {
            double modifier = 2.52 / config.sensitivity;

            if (IsKeyHolded(VK_LBUTTON))
            {
                WeaponData data = GetWeaponData(weapon, modifier);
                HandleWeaponFire(arduino, data.x, data.y, data.delay);
            }

            ProcessKeyEvents(arduino, config);
        }
    }
}