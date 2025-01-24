#include <windows.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

#include "Utils.h"
#include "Config.h"
#include "Arduino.h"
#include "Weapons.h"
#include "AsciiArt.h"
#include "ColorBot.h"
#include "FastReload.h"

Weapon weapon = OFF;

// Function to simulate human-like mouse movement with inertia and randomness
static void SimulateHumanLikeMovement(const Arduino& arduino, int totalX, int totalY, int baseDelay)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> stepsDist(8, 12); // Random step count
    int steps = stepsDist(gen);

    std::uniform_real_distribution<double> delayMultDist(0.8, 1.2); // Random delay multiplier
    std::uniform_real_distribution<double> offsetDist(-1.0, 1.0);   // Small random offset

    double stepX = static_cast<double>(totalX) / steps;
    double stepY = static_cast<double>(totalY) / steps;

    double accumulatedX = 0.0;
    double accumulatedY = 0.0;

    for (int i = 0; i < steps; i++)
    {
        double moveXf = stepX + offsetDist(gen);
        double moveYf = stepY + offsetDist(gen);

        accumulatedX += moveXf;
        accumulatedY += moveYf;

        if (i == steps - 1)
        {
            moveXf += (totalX - static_cast<int>(accumulatedX));
            moveYf += (totalY - static_cast<int>(accumulatedY));
        }

        int moveX = static_cast<int>(moveXf);
        int moveY = static_cast<int>(moveYf);
        int currentDelay = static_cast<int>(baseDelay * delayMultDist(gen));

        arduino.WriteMessage(
            "MOUSE_LEFT_HOLDED:" +
            std::to_string(moveX) + "," +
            std::to_string(moveY) + "," +
            std::to_string(currentDelay)
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(currentDelay));
    }
}

// Function to handle weapon fire with optional human-like movement
static void HandleWeaponFire(const Arduino& arduino, Weapon& weapon, const Config& config, const FastReload& fastReload)
{
    if (weapon == OFF) return;

    double obs = 0.75 / config.GetZoomSensitivity();
    double modifier = 2.52 / config.GetSensitivity();
    modifier = IsKeyHolded(VK_RBUTTON) ? modifier * obs : modifier;

    WeaponData data = GetWeaponData(weapon, modifier);

    if (!(data.x.size() == data.y.size() && data.x.size() == data.delay.size()))
    {
        weapon = OFF;
        return;
    }

    bool useHumanizedMovement = config.GetHumanizedMovement(); 
    // Set this to true to enable humanized movement, false to disable

    for (size_t i = 0; i < data.x.size(); i++)
    {
        if (!IsKeyHolded(VK_LBUTTON) || 
            (config.GetConfirmationKey() != 0 && !IsKeyHolded(config.GetConfirmationKey())))
        {
            return;
        }

        if (useHumanizedMovement)
        {
            SimulateHumanLikeMovement(
                arduino,
                static_cast<int>(data.x[i]),
                static_cast<int>(data.y[i]),
                static_cast<int>(data.delay[i])
            );
        }
        else
        {
            arduino.WriteMessage(
                "MOUSE_LEFT_HOLDED:" +
                std::to_string(static_cast<int>(data.x[i])) + "," +
                std::to_string(static_cast<int>(data.y[i])) + "," +
                std::to_string(data.delay[i])
            );
            std::this_thread::sleep_for(std::chrono::milliseconds(data.delay[i]));
        }
    }

    arduino.WriteMessage("MOUSE_LEFT_CLICK");

    if (config.GetFastReload() != 0)
    {
        fastReload.Process(arduino, weapon);
    }
}

// Function to process key events for additional actions like bhop and rapid fire
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
    utils.PrintAscii(ASCII_OUTRO);
    utils.PrintHotkeys(ASCII_HOTKEYS);

    FastReload fastReload;
    ColorBot colorBot(config.GetColorBotThreshold(), config.GetColorBotKey());

    while (true)
    {
        weapon = GetWeaponState(weapon);

        HandleWeaponFire(arduino, weapon, config, fastReload);
        ProcessKeyEvents(arduino, config, colorBot);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
