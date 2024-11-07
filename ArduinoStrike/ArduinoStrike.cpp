#include "Utils.h"
#include "Config.h"
#include "Arduino.h"
#include "Weapons.h"
#include "AsciiArt.h"
#include "ColorBot.h"

Weapon weapon = OFF;

static void HandleWeaponFire(const Arduino& arduino, Weapon& weapon, const Config& config)
{
    if (weapon == OFF) return;

    double obs = 0.75 / config.GetZoomSensitivity();
    double modifier = 2.52 / config.GetSensitivity();
    modifier = IsKeyHolded(VK_RBUTTON) ? modifier *= obs : modifier;

    WeaponData data = GetWeaponData(weapon, modifier);

    if (!(data.x.size() == data.y.size() && data.x.size() == data.delay.size()))
    {
        //cerr << "Warning: Mismatch in the number of elements across x, y, and delay vectors. All three vectors must have the same number of elements." << endl;
        weapon = OFF;
        return;
    }

    for (size_t i = 0; i < data.x.size(); i++)
    {
        if (!IsKeyHolded(VK_LBUTTON) || (config.GetConfirmationKey() != 0 && !IsKeyHolded(config.GetConfirmationKey())))
        {
            break;
        }

        arduino.WriteMessage("MOUSE_LEFT_HOLDED:" + to_string(data.x[i]) + "," + to_string(data.y[i]) + "," + to_string(data.delay[i]));
        sleep_for(milliseconds(data.delay[i]));
    }
}

static void ProcessKeyEvents(const Arduino& arduino, const Config& config, const ColorBot& colorBot)
{
    if (IsKeyHolded(VK_SPACE) && config.GetBhop() != 0)
    {
        arduino.WriteMessage("SPACE_BUTTON_HOLDED");
    }

    if (IsKeyHolded(VK_MBUTTON) && config.GetRapidFire() != 0)
    {
        arduino.WriteMessage("MOUSE_MIDDLE_HOLDED");
    }

    if (IsKeyHolded(config.GetColorBotKey()) && config.GetColorBotKey() != 0)
    {
        colorBot.Process(arduino);
    }
}

int main()
{
    Utils utils;
    Config config;

    utils.PrintAscii(ASCII_INTRO);
    Arduino arduino("Arduino Leonardo");
    utils.PrintAscii(ASCII_OUTRO), utils.PrintHotkeys(ASCII_HOTKEYS);

    ColorBot colorBot(config.GetColorBotThreshold(), config.GetColorBotKey());

    while (true)
    {
        weapon = GetWeaponState(weapon);
        string message = arduino.ReceiveMessage('\n');

        if (message.rfind("ARDUINO_INITIATED", 0) != 0)
        {
            HandleWeaponFire(arduino, weapon, config);
            ProcessKeyEvents(arduino, config, colorBot);
        }
    }
}