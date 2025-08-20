#include "Config.h"

const json Config::DEFAULT_CONFIG =
{
    { "umpKey", 0x70 },
    { "m4a1Key", 0x71 },
    { "m4a4Key", 0x72 },
    { "ak47Key", 0x73 },
    { "galilKey", 0x74 },
    { "famasKey", 0x75 },
    { "augKey", 0x76 },
    { "sgKey", 0x77 },
    { "offKey", 0x7B },

    { "bhop", false },
    { "rapidFire", true },
    { "fastReload", false },

    { "sensitivity", 8.0 },
    { "zoomSensitivity", 1.0 },
    { "colorThreshold", 20 },

    { "confirmationKey", 0x00 },
    { "colorBotKey", 0x58 },
    { "autoAcceptKey", 0x48 }
};

Config::Config()
{
    if (!Validate(DEFAULT_CONFIG))
    {
        throw runtime_error("Invalid default config");
    }

    Load();
}

void Config::Load()
{
    string message;
    ifstream file("Config.json");

    if (file.is_open() && !file.fail())
    {
        try
        {
            json j;
            file >> j;

            if (!Validate(j))
            {
                throw runtime_error("Config validation failed");
            }

            FromJson(j);
        }
        catch (const json::parse_error& e)
        {
            message = "[!] JSON syntax error: " + string(e.what());
        }
        catch (const exception& e)
        {
            message = "[!] Config error: " + string(e.what());
        }
        catch (...)
        {
            message = "[!] Unknown error occurred";
        }

        if (!message.empty())
        {
            file.close();

            if (remove("Config.json") != 0)
            {
                char error_msg[256];
                strerror_s(error_msg, sizeof(error_msg), errno);
                cerr << "[!] Failed to delete config: " << error_msg << endl;
            }

            cerr << message << endl;
            InteractiveSetup();
        }
    }
    else
    {
        InteractiveSetup();
    }
}

bool Config::Validate(const json& j) const
{
    const vector<string> REQUIRED_KEYS =
    {
        "umpKey",
        "m4a1Key",
        "m4a4Key",
        "ak47Key",
        "galilKey",
        "famasKey",
        "augKey",
        "sgKey",
        "offKey",
        "bhop",
        "rapidFire",
        "fastReload",
        "sensitivity",
        "zoomSensitivity",
        "colorThreshold",
        "confirmationKey",
        "colorBotKey",
        "autoAcceptKey"
    };

    // Verification of required keys
    for (const auto& key : REQUIRED_KEYS)
    {
        if (!j.contains(key))
        {
            cerr << "[!] Missing required key: " << key << endl;
            return false;
        }
    }

    // Auxiliary function for type conversion to string
    auto TypeToString = [](json::value_t type) -> string
        {
            switch (type)
            {
            case json::value_t::boolean: return "boolean";
            case json::value_t::number_float: return "float";
            case json::value_t::number_integer: return "integer";
            case json::value_t::number_unsigned: return "unsigned";
            case json::value_t::string: return "string";
            default: return "unknown";
            }
        };

    try
    {
        bool valid = true;

        const vector<pair<string, json::value_t>> TYPE_CHECKS =
        {
            { "bhop", json::value_t::boolean },
            { "rapidFire", json::value_t::boolean },
            { "fastReload", json::value_t::boolean }
        };

        // Type checking
        for (const auto& [key, type] : TYPE_CHECKS)
        {
            if (j[key].type() != type)
            {
                cerr << "[!] Invalid type for '" << key << "'. Expected: " << TypeToString(type) << endl;
                valid = false;
            }
        }

        const vector<tuple<string, double, double>> RANGE_CHECKS =
        {
            { "umpKey", 0, 0xFE },
            { "m4a1Key", 0, 0xFE },
            { "m4a4Key", 0, 0xFE },
            { "ak47Key", 0, 0xFE },
            { "galilKey", 0, 0xFE },
            { "famasKey", 0, 0xFE },
            { "augKey", 0, 0xFE },
            { "sgKey", 0, 0xFE },
            { "offKey", 0, 0xFE },
            { "sensitivity", 1.0, 8.0 },
            { "zoomSensitivity", 0.01, 3.0 },
            { "colorThreshold", 0, 20 },
            { "confirmationKey", 0, 0xFE },
            { "colorBotKey", 0, 0xFE },
            { "autoAcceptKey", 0, 0xFE }
        };

        // Checking ranges
        for (const auto& [key, min, max] : RANGE_CHECKS)
        {
            const double value = j[key].get<double>();

            if (value < min || value > max)
            {
                cerr << "[!] Invalid value for " << key << ": " << value << " (valid range: " << min << "-" << max << ")" << endl;
                valid = false;
            }
        }

        return valid;
    }
    catch (const json::exception& e)
    {
        cerr << "[!] JSON parsing error: " << e.what() << endl;
        return false;
    }
}

string Config::GenerateHotkeysString() const
{
    const vector<pair<function<int()>, string>> binds =
    {
        { [this]() { return offKey;   }, "OFF"      },
        { [this]() { return umpKey;   }, "UMP-45"   },
        { [this]() { return m4a1Key;  }, "M4A1-S"   },
        { [this]() { return m4a4Key;  }, "M4A4"     },
        { [this]() { return ak47Key;  }, "AK-47"    },
        { [this]() { return galilKey; }, "GALIL-AR" },
        { [this]() { return famasKey; }, "FAMAS"    },
        { [this]() { return augKey;   }, "AUG"      },
        { [this]() { return sgKey;    }, "SG-553"   }
    };

    string result;

    for (const auto& [getter, name] : binds)
    {
        if (!result.empty()) result += " | ";
        result += "[" + KeyCodeToString(getter()) + "] - " + name;
    }

    return result;
}

void Config::InteractiveSetup()
{
    cout << endl << "=== ArduinoStrike config wizard ===" << endl;

    // Step 1: Offering a standard configuration
    if (OfferDefaultConfig()) return;

    // Step 2: Manual Setup
    cout << endl << "Let's create your perfect configuration!" << endl;
    ConfigureFeatures();
    ConfigureSettings();
    ConfigureKeys();
    PrintSuccess();
    Save();

    cout << endl << "[+] Configuration complete! Happy gaming!" << endl;
}

bool Config::OfferDefaultConfig()
{
    cout << "\nWould you like to use the default settings recommended by the author?\n"
        << "These settings are optimized for most players.\n"
        << "Enter Y to show defaults or N to customize settings.\n"
        << "Show default settings? (Y/N): ";

    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(choice) == 'Y')
    {
        // Saving the current configuration so that it can be rolled back
        Config backup = *this;

        // Applying the default settings
        FromJson(DEFAULT_CONFIG);
        PrintSuccess();

        cout << endl << "Keep these settings? (Y/N): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (toupper(choice) == 'Y')
        {
            Save();
            cout << endl << "[+] Default configuration applied!" << endl;
            return true;
        }

        // If the user refused, we roll back to the old settings
        *this = backup;
    }

    return false;
}


void Config::ConfigureFeatures()
{
    cout << endl << "--- FEATURE SELECTION ---" << endl;

    const array<tuple<string, string, bool*>, 3> features =
    {
        {
            { "Bunny Hop", "Automatically jump while holding spacebar for faster movement.", &bhop },
            { "Rapid Fire", "Increase fire rate of semi-automatic weapons for quicker shots.", &rapidFire },
            { "Fast Reload", "Reduce reload time for certain weapons to resume firing faster.", &fastReload }
        }
    };

    auto getUserResponse = []() -> bool
        {
            char input;

            while (true)
            {
                cin >> input;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                input = toupper(input);
                if (input == 'Y') return true;
                if (input == 'N') return false;
                cout << "Invalid input! Please enter Y or N: ";
            }
        };

    for (const auto& [name, description, target] : features)
    {
        cout << endl << "> " << name << " <" << endl << description << endl << "Enable? (Y/N): ";
        *target = getUserResponse();
    }
}

void Config::ConfigureSettings()
{
    cout << endl << "--- PARAMETER SETTINGS ---" << endl;

    auto GetValidatedInput = [](const auto& prompt, auto min, auto max)
        {
            using T = decltype(min);
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

    sensitivity = GetValidatedInput(
        "Enter your mouse sensitivity for general gameplay.\n"
        "This controls how fast your cursor moves in the game.\n"
        "Range: 1.0 (slow) to 8.0 (fast).\n"
        "Example: 8.0\n> ", 1.0, 8.0);

    zoomSensitivity = GetValidatedInput(
        "Enter your mouse sensitivity when zoomed in (e.g., aiming down sights).\n"
        "This controls how fast your cursor moves while zooming.\n"
        "Range: 0.01 (very slow) to 3.00 (very fast).\n"
        "Example: 1.00\n> ", 0.01, 3.0);

    colorThreshold = GetValidatedInput(
        "Enter the Color Bot threshold.\n"
        "This sets how sensitive the Color Bot is to color differences.\n"
        "Lower values mean stricter matching, higher values allow more variation.\n"
        "Range: 0 (exact match) to 20 (maximum variation).\n"
        "Example: 20\n> ", 0, 20);
}

void Config::ConfigureKeys()
{
    cout << endl << "--- HOTKEY BINDING SETUP ---" << endl;

    auto BindKey = [this](const string& action, int& key)
        {
            while (true)
            {
                cout << "\nPress key for [" << action << "] or ESC to turn off the hotkey..." << endl;
                int hotkey = WaitForKeyPress();

                if (hotkey == VK_ESCAPE)
                {
                    cout << "Hotkey for [" << action << "] disabled!" << endl;
                    return;
                }

                if (hotkey == 0)
                {
                    cout << "Hotkey input timeout! Try again." << endl;
                    continue;
                }

                cout << "-> " << KeyCodeToString(hotkey) << endl;
                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
                cout << "Confirm this hotkey? (Y/N): ";

                char confirm;
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (toupper(confirm) == 'Y')
                {
                    key = hotkey;
                    break;
                }
            }
        };

    vector<pair<string, reference_wrapper<int>>> bindings =
    {
        { "Recoil Control Confirmation", ref(confirmationKey) },
        { "Color Bot", ref(colorBotKey) },
        { "Auto Accept", ref(autoAcceptKey) },
        { "UMP-45", ref(umpKey) },
        { "M4A1-S", ref(m4a1Key) },
        { "M4A4", ref(m4a4Key) },
        { "AK-47", ref(ak47Key) },
        { "Galil AR", ref(galilKey) },
        { "FAMAS", ref(famasKey) },
        { "AUG", ref(augKey) },
        { "SG-553", ref(sgKey) },
        { "OFF", ref(offKey) }
    };

    for (const auto& binding : bindings)
    {
        BindKey(binding.first, binding.second.get());
    }
}

int Config::WaitForKeyPress() const
{
    while (GetAsyncKeyState(VK_RETURN) & 0x8000);
    cout << "Waiting for key press..." << endl;

    while (true)
    {
        for (int key = 1; key < 256; key++)
        {
            if (GetAsyncKeyState(key) & 0x8000)
            {
                while (GetAsyncKeyState(key) & 0x8000);
                return key;
            }
        }

        Sleep(10);
    }
}

string Config::KeyCodeToString(int code) const
{
    if (code == 0) return "NONE";

    // Let's try to get the key name via GetKeyNameTextA
    char name[128] = { 0 };
    UINT scanCode = MapVirtualKey(code, MAPVK_VK_TO_VSC);

    // If the code contains an extended bit, it must be set manually
    LONG lParam = scanCode << 16;

    // Some keys require setting an extended flag (for example, arrows, numpad, etc.)
    if (code == VK_RIGHT || code == VK_LEFT || code == VK_UP || code == VK_DOWN || code == VK_INSERT || code == VK_DELETE || code == VK_HOME || code == VK_END || code == VK_PRIOR || code == VK_NEXT)
    {
        lParam |= 0x01000000;
    }

    if (GetKeyNameTextA(lParam, name, sizeof(name)) != 0)
    {
        return string(name);
    }

    // If GetKeyNameTextA could not get a name, we use a pre-prepared dictionary
    static const unordered_map<int, string> names =
    {
        { VK_LBUTTON, "LMB" },
        { VK_RBUTTON, "RMB" },
        { VK_CANCEL, "Break" },
        { VK_MBUTTON, "MMB" },
        { VK_XBUTTON1, "MOUSE4" },
        { VK_XBUTTON2, "MOUSE5" },
        { VK_BACK, "Backspace" },
        { VK_TAB, "Tab" },
        { VK_CLEAR, "Clear" },
        { VK_RETURN, "Enter" },
        { VK_SHIFT, "Shift" },
        { VK_CONTROL, "Ctrl" },
        { VK_MENU, "Alt" },
        { VK_PAUSE, "Pause" },
        { VK_CAPITAL, "Caps Lock" },
        { VK_ESCAPE, "Escape" },
        { VK_SPACE, "Space" },
        { VK_PRIOR, "Page Up" },
        { VK_NEXT, "Page Down" },
        { VK_END, "End" },
        { VK_HOME, "Home" },
        { VK_LEFT, "Left Arrow" },
        { VK_UP, "Up Arrow" },
        { VK_RIGHT, "Right Arrow" },
        { VK_DOWN, "Down Arrow" },
        { VK_SELECT, "Select" },
        { VK_PRINT, "Print" },
        { VK_EXECUTE, "Execute" },
        { VK_SNAPSHOT, "Print Screen" },
        { VK_INSERT, "Insert" },
        { VK_DELETE, "Delete" },
        { VK_HELP, "Help" },
        { VK_F1, "F1" },
        { VK_F2, "F2" },
        { VK_F3, "F3" },
        { VK_F4, "F4" },
        { VK_F5, "F5" },
        { VK_F6, "F6" },
        { VK_F7, "F7" },
        { VK_F8, "F8" },
        { VK_F9, "F9" },
        { VK_F10, "F10" },
        { VK_F11, "F11" },
        { VK_F12, "F12" },
        { VK_F13, "F13" },
        { VK_F14, "F14" },
        { VK_F15, "F15" },
        { VK_F16, "F16" },
        { VK_F17, "F17" },
        { VK_F18, "F18" },
        { VK_F19, "F19" },
        { VK_F20, "F20" },
        { VK_F21, "F21" },
        { VK_F22, "F22" },
        { VK_F23, "F23" },
        { VK_F24, "F24" },
        { VK_NUMLOCK, "Num Lock" },
        { VK_SCROLL, "Scroll Lock" },
        { VK_NUMPAD0, "Numpad 0" },
        { VK_NUMPAD1, "Numpad 1" },
        { VK_NUMPAD2, "Numpad 2" },
        { VK_NUMPAD3, "Numpad 3" },
        { VK_NUMPAD4, "Numpad 4" },
        { VK_NUMPAD5, "Numpad 5" },
        { VK_NUMPAD6, "Numpad 6" },
        { VK_NUMPAD7, "Numpad 7" },
        { VK_NUMPAD8, "Numpad 8" },
        { VK_NUMPAD9, "Numpad 9" },
        { VK_MULTIPLY, "Multiply" },
        { VK_ADD, "Add" },
        { VK_SEPARATOR, "Separator" },
        { VK_SUBTRACT, "Subtract" },
        { VK_DECIMAL, "Decimal" },
        { VK_DIVIDE, "Divide" },
        { VK_OEM_1, ";:" },
        { VK_OEM_PLUS, "+=" },
        { VK_OEM_COMMA, ",<" },
        { VK_OEM_MINUS, "-_" },
        { VK_OEM_PERIOD, ".>" },
        { VK_OEM_2, "/?" },
        { VK_OEM_3, "`~" },
        { VK_OEM_4, "[{" },
        { VK_OEM_5, "\\|" },
        { VK_OEM_6, "]}" },
        { VK_OEM_7, "'\"" },
        { VK_OEM_8, "OEM_8" },
        { VK_OEM_102, "<>" },
        { VK_LWIN, "LWIN" },
        { VK_RWIN, "RWIN" },
        { VK_APPS, "Applications" }
    };

    auto iterator = names.find(code);

    if (iterator != names.end())
    {
        return iterator->second;
    }

    return "UNKNOWN";
}


void Config::PrintSuccess() const
{
    cout << endl << "--- FINAL CONFIGURATION ---" << endl;

    auto printLine = [](const string& label, const string& value)
        {
            cout << label << ": " << value << endl;
        };

    printLine("Bunny Hop", (bhop ? "ENABLED" : "DISABLED"));
    printLine("Rapid Fire", (rapidFire ? "ENABLED" : "DISABLED"));
    printLine("Fast Reload", (fastReload ? "ENABLED" : "DISABLED"));

    printLine("Sensitivity", FormatFloat(sensitivity));
    printLine("Zoom Sensitivity", FormatFloat(zoomSensitivity));

    printLine("Color Bot Threshold", to_string(colorThreshold));

    printLine("Recoil Control Confirmation", KeyCodeToString(confirmationKey));
    printLine("Color Bot Key", KeyCodeToString(colorBotKey));
    printLine("Auto Accept Key", KeyCodeToString(autoAcceptKey));

    cout << endl;
    printLine("UMP-45", KeyCodeToString(umpKey));
    printLine("M4A1-S", KeyCodeToString(m4a1Key));
    printLine("M4A4", KeyCodeToString(m4a4Key));
    printLine("AK-47", KeyCodeToString(ak47Key));
    printLine("Galil", KeyCodeToString(galilKey));
    printLine("FAMAS", KeyCodeToString(famasKey));
    printLine("AUG", KeyCodeToString(augKey));
    printLine("SG-553", KeyCodeToString(sgKey));
    printLine("OFF", KeyCodeToString(offKey));

    cout << "---------------------------" << endl;
}

json Config::ToJson() const
{
    return
    {
        { "bhop", bhop },
        { "rapidFire", rapidFire },
        { "fastReload", fastReload },

        { "sensitivity", sensitivity },
        { "zoomSensitivity", zoomSensitivity },
        { "colorThreshold", colorThreshold },

        { "confirmationKey", confirmationKey },
        { "colorBotKey", colorBotKey },
        { "autoAcceptKey", autoAcceptKey },

        { "umpKey", umpKey },
        { "m4a1Key", m4a1Key },
        { "m4a4Key", m4a4Key },
        { "ak47Key", ak47Key },
        { "galilKey", galilKey },
        { "famasKey", famasKey },
        { "augKey", augKey },
        { "sgKey", sgKey },
        { "offKey", offKey }
    };
}

void Config::FromJson(const json& j)
{
    auto readKey = [this, &j](const string& key, auto& var)
        {
            var = j.value(key, DEFAULT_CONFIG.at(key));
        };

    readKey("bhop", bhop);
    readKey("rapidFire", rapidFire);
    readKey("fastReload", fastReload);

    readKey("sensitivity", sensitivity);
    readKey("zoomSensitivity", zoomSensitivity);
    readKey("colorThreshold", colorThreshold);

    readKey("confirmationKey", confirmationKey);
    readKey("colorBotKey", colorBotKey);
    readKey("autoAcceptKey", autoAcceptKey);

    readKey("umpKey", umpKey);
    readKey("m4a1Key", m4a1Key);
    readKey("m4a4Key", m4a4Key);
    readKey("ak47Key", ak47Key);
    readKey("galilKey", galilKey);
    readKey("famasKey", famasKey);
    readKey("augKey", augKey);
    readKey("sgKey", sgKey);
    readKey("offKey", offKey);
}

string Config::FormatFloat(double value, int precision)
{
    ostringstream oss;
    oss << fixed << setprecision(precision) << value;
    return oss.str();
}

void Config::Save() const
{
    ofstream file("Config.json");
    file << setw(4) << ToJson() << endl;
}