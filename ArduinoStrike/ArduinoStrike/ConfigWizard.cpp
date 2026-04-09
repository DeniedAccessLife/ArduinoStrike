#include "ConfigWizard.h"

#include <array>
#include <tuple>
#include <limits>
#include <utility>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace ConfigSchema;

ConfigWizard::ConfigWizard(Config& cfg) : config(cfg) { }

void ConfigWizard::Run()
{
    cout << endl << "=== ArduinoStrike config wizard ===" << endl;

    // Step 1: Offering a standard configuration
    if (OfferDefaultConfig()) return;

    // Step 2: Manual Setup
    cout << endl << "Let's create your perfect configuration!" << endl;
    ConfigureFeatures();
    ConfigureSettings();
    ConfigureKeys();
    config.PrintSuccess();
    config.Save();

    cout << endl << "[+] Configuration complete! Happy gaming!" << endl;
}

bool ConfigWizard::OfferDefaultConfig()
{
    cout << "\nWould you like to use the default settings recommended by the author?\n"
        << "These settings are optimized for most players.\n"
        << "Enter Y to show defaults or N to customize settings.\n"
        << "Show default settings? (Y/N): ";

    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(static_cast<unsigned char>(choice)) == 'Y')
    {
        // Saving the current configuration so that it can be rolled back
        Config backup = config;

        // Applying the default settings
        config.FromJson(Config::DEFAULT_CONFIG);
        config.PrintSuccess();

        cout << endl << "Keep these settings? (Y/N): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (toupper(static_cast<unsigned char>(choice)) == 'Y')
        {
            config.Save();
            cout << endl << "[+] Default configuration applied!" << endl;
            return true;
        }

        // If the user refused, we roll back to the old settings
        config = backup;
    }

    return false;
}

void ConfigWizard::ConfigureFeatures()
{
    cout << endl << "--- FEATURE SELECTION ---" << endl;

    const array<tuple<const char*, const char*, bool*>, 3> features =
    { {
        { "Bunny Hop", "Automatically jump while holding spacebar for faster movement.", &config.bhop },
        { "Rapid Fire", "Increase fire rate of semi-automatic weapons for quicker shots.", &config.rapidFire },
        { "Fast Reload", "Reduce reload time for certain weapons to resume firing faster.", &config.fastReload }
    } };

    auto getUserResponse = []() -> bool
    {
        char input;

        while (true)
        {
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = static_cast<char>(toupper(static_cast<unsigned char>(input)));
            if (input == 'Y') return true;
            if (input == 'N') return false;
            cout << "Invalid input! Please enter Y or N: ";
        }
    };

    for (const auto& feature : features)
    {
        const char* name = get<0>(feature);
        const char* description = get<1>(feature);
        bool* target = get<2>(feature);

        cout << endl << "> " << name << " <" << endl << description << endl << "Enable? (Y/N): ";
        *target = getUserResponse();
    }
}

void ConfigWizard::ConfigureSettings()
{
    cout << endl << "--- PARAMETER SETTINGS ---" << endl;

    auto GetValidatedInput = [](const auto& prompt, auto min, auto max)
    {
        using T = decay_t<decltype(min)>;
        T value{};

        while (true)
        {
            cout << endl << prompt;

            if (!(cin >> value))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (value >= min && value <= max)
            {
                return value;
            }

            cout << "Invalid value! Must be between " << min << " and " << max << endl;
        }
    };

    config.sensitivity = GetValidatedInput(
        "Enter your mouse sensitivity for general gameplay.\n"
        "This controls how fast your cursor moves in the game.\n"
        "Range: 1.0 (slow) to 8.0 (fast).\n"
        "Example: 8.0\n> ", 1.0, 8.0);

    config.zoomSensitivity = GetValidatedInput(
        "Enter your mouse sensitivity when zoomed in (e.g., aiming down sights).\n"
        "This controls how fast your cursor moves while zooming.\n"
        "Range: 0.01 (very slow) to 3.00 (very fast).\n"
        "Example: 1.00\n> ", 0.01, 3.0);

    config.colorThreshold = GetValidatedInput(
        "Enter the Color Bot threshold.\n"
        "This sets how sensitive the Color Bot is to color differences.\n"
        "Lower values mean stricter matching, higher values allow more variation.\n"
        "Range: 0 (exact match) to 20 (maximum variation).\n"
        "Example: 20\n> ", 0, 20);

    config.turnAroundDistance = GetValidatedInput(
        "Enter the Turn Around distance.\n"
        "This allows you to control the movement of the mouse when rotating.\n"
        "Arduino HID bypasses Windows acceleration, so this must be calibrated manually.\n"
        "Tip: face a wall, press the hotkey twice — you should end up in the exact same spot.\n"
        "Overshot (turned too far)  -> lower the value.\n"
        "Undershot (turned too little) -> raise the value.\n"
        "Range: 100 (very short) to 10000 (very far).\n"
        "Example: 1022\n> ", 100, 10000);
}

void ConfigWizard::ConfigureKeys()
{
    using std::ref;
    using std::reference_wrapper;

    cout << endl << "--- HOTKEY BINDING SETUP ---" << endl;

    auto BindKey = [this](const string& action, int& key)
    {
        while (true)
        {
            cout << "\nPress key for [" << action << "] or ESC to turn off the hotkey..." << endl;
            int hotkey = config.WaitForKeyPress();

            if (hotkey == VK_ESCAPE)
            {
                cout << "Hotkey for [" << action << "] disabled!" << endl;
                key = 0x00;
                return;
            }

            if (hotkey == 0)
            {
                cout << "Hotkey input timeout! Try again." << endl;
                continue;
            }

            cout << "-> " << config.KeyCodeToString(hotkey) << endl;
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            cout << "Confirm this hotkey? (Y/N): ";

            char confirm;
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (toupper(static_cast<unsigned char>(confirm)) == 'Y')
            {
                key = hotkey;
                break;
            }
        }
    };

    // Configure weapon keys
    for (const auto& info : WEAPONS)
    {
        int& key = config.weaponKeys[info.weapon];
        BindKey(info.display, key);
    }

    // Configure other keys
    vector<pair<string, reference_wrapper<int>>> otherBindings =
    {
        { "Recoil Control Confirmation", ref(config.confirmationKey) },
        { "Color Bot", ref(config.colorBotKey) },
        { "Auto Accept", ref(config.autoAcceptKey) },
        { "Turn Around", ref(config.turnAroundKey) }
    };

    for (const auto& binding : otherBindings)
    {
        BindKey(binding.first, binding.second.get());
    }
}