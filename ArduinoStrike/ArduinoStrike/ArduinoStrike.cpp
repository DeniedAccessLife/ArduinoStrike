#include "Bhop.h"
#include "Utils.h"
#include "Module.h"
#include "Config.h"
#include "Logger.h"
#include "Arduino.h"
#include "Weapons.h"
#include "AsciiArt.h"
#include "ColorBot.h"
#include "RapidFire.h"
#include "AutoAccept.h"
#include "FastReload.h"
#include "ModuleManager.h"
#include "RecoilControl.h"
volatile bool g_shouldExit = false;

static BOOL WINAPI ConsoleHandler(DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_C_EVENT)
    {
        Logger::LogMessage("Received CTRL+C. Shutting down...");
        g_shouldExit = true;
        return TRUE;
    }

    return FALSE;
}

int main()
{
    Logger::Init();

    Utils utils;
    Config config;

    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
    {
        Logger::LogMessage("Failed to set console control handler!", boost::log::trivial::error);
    }

    utils.PrintAscii(ASCII_INTRO);
    
    Arduino* arduino = nullptr;

    try
    {
        arduino = new Arduino("Arduino Leonardo");
    }
    catch (const runtime_error& e)
    {
        Logger::LogMessage("Failed to initialize Arduino: " + string(e.what()));
        Logger::LogMessage("Exiting...");
        return 1;
    }
    
    utils.PrintAscii(ASCII_OUTRO);
    utils.PrintHotkeys(config.GenerateHotkeysString());

    ModuleManager manager;
    manager.AddModule<Bhop>("Bhop", VK_SPACE);
    manager.AddModule<FastReload>("FastReload");
    manager.AddModule<RapidFire>("RapidFire", VK_MBUTTON);
    manager.AddModule<RecoilControl>("RecoilControl", manager);
    manager.AddModule<AutoAccept>("AutoAccept", config.GetAutoAcceptKey());
    manager.AddModule<ColorBot>("ColorBot", config.GetColorThreshold(), config.GetColorBotKey());

    while (!g_shouldExit)
    {
        manager.ProcessModules(*arduino, config);
        sleep_for(milliseconds(10));
    }
    
    delete arduino;
}