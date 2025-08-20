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

int main(int argc, char* argv[])
{
    Logger::Init();

    Utils utils;
    Config config;
    
    Logger::LogMessage("Main: argc = " + to_string(argc));
    for (int i = 0; i < argc; i++)
    {
        Logger::LogMessage("Main: argv[" + to_string(i) + "] = " + string(argv[i]));
    }
    
    bool dry_run = false;
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        Logger::LogMessage("Main: Checking argument: " + arg);
        
        if (arg == "--dry-run" || arg == "-d")
        {
            dry_run = true;
            Logger::LogMessage("Main: Dry run mode enabled via command line argument!");
        }
    }
    
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
    {
        Logger::LogMessage("Failed to set console control handler!", boost::log::trivial::error);
    }

    utils.PrintAscii(ASCII_INTRO);
    
    Arduino* arduino = nullptr;

    try
    {
        arduino = new Arduino("Arduino Leonardo", dry_run);
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