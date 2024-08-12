#include "Utils.h"
#include "Arduino.h"
#include "Weapons.h"
#include "AsciiArt.h"

Weapon weapon = OFF;

static void HandleWeaponFire(const Arduino& arduino, const vector<double>& x, const vector<double>& y, const vector<int>& delay, const Config& config)
{
    for (size_t i = 0; i < x.size(); i++)
    {
        bool isRecoilControlActive = IsKeyHolded(VK_LBUTTON) && (config.confirmationKey == 0 || IsKeyHolded(config.confirmationKey));

        if (isRecoilControlActive)
        {
            arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(x[i]) + "," + to_string(y[i]) + "," + to_string(delay[i]));
            sleep_for(milliseconds(delay[i]));
        }
    }
}

static void ProcessKeyEvents(const Arduino& arduino, const Config& config)
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
            bool isRecoilControlActive = IsKeyHolded(VK_LBUTTON) && (config.confirmationKey == 0 || IsKeyHolded(config.confirmationKey));

            if (isRecoilControlActive)
            {
                WeaponData data = GetWeaponData(weapon, modifier);
                HandleWeaponFire(arduino, data.x, data.y, data.delay, config);
            }

            ProcessKeyEvents(arduino, config);
        }
    }
}
