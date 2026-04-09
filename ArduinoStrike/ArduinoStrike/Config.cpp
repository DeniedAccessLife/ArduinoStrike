#include "Config.h"
#include "Weapons.h"
#include "ConfigSchema.h"
#include "ConfigWizard.h"

using namespace std;
using namespace ConfigSchema;

std::string Config::WeaponToString(Weapon weapon)
{
    if (const auto* info = FindWeaponInfo(weapon))
    {
        return info->id;
    }
    return "";
}

Weapon Config::WeaponFromString(const std::string& name)
{
    if (const auto* info = FindWeaponInfo(name))
    {
        return info->weapon;
    }
    return OFF;
}

int Config::GetWeaponKey(Weapon weapon) const
{
    auto it = weaponKeys.find(weapon);
    return it != weaponKeys.end() ? it->second : 0x00;
}

int Config::GetUMPKey() const { return GetWeaponKey(UMP); }
int Config::GetM4A1Key() const { return GetWeaponKey(M4A1); }
int Config::GetM4A4Key() const { return GetWeaponKey(M4A4); }
int Config::GetAK47Key() const { return GetWeaponKey(AK47); }
int Config::GetGALILKey() const { return GetWeaponKey(GALIL); }
int Config::GetFAMASKey() const { return GetWeaponKey(FAMAS); }
int Config::GetAUGKey() const { return GetWeaponKey(AUG); }
int Config::GetSGKey() const { return GetWeaponKey(SG); }
int Config::GetOFFKey() const { return GetWeaponKey(OFF); }

static WeaponData CreateDefaultWeaponData(const vector<double>& x, const vector<double>& y, const vector<int>& delay)
{
    WeaponData result;
    result.x = x;
    result.y = y;
    result.delay = delay;
    return result;
}

static json WeaponDataToJson(const WeaponData& data)
{
    return {
        { "x", data.x },
        { "y", data.y },
        { "delay", data.delay }
    };
}

static WeaponData WeaponDataFromJson(const json& j)
{
    WeaponData data;
    if (j.contains("x") && j["x"].is_array())
    {
        for (const auto& val : j["x"])
            data.x.push_back(val.get<double>());
    }
    if (j.contains("y") && j["y"].is_array())
    {
        for (const auto& val : j["y"])
            data.y.push_back(val.get<double>());
    }
    if (j.contains("delay") && j["delay"].is_array())
    {
        for (const auto& val : j["delay"])
            data.delay.push_back(val.get<int>());
    }
    return data;
}

static json CreateDefaultWeaponRecoilData()
{
    return {
        { "UMP", WeaponDataToJson(CreateDefaultWeaponData(
            { -1, -4, -2, -4, -9, -3, 11, -4, 9, 18, 15, -1, 5, 0, 9, 5, -12, -19, -1, 15, 17, -6, -20, -3, -3 },
            { 6, 8, 18, 23, 23, 26, 17, 12, 13, 8, 5, 3, 6, 6, -3, -1, 4, 1, -2, -5, -2, 3, -2, -1, -1 },
            { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 85, 90, 90, 85, 90, 90, 90, 90 }
        )) },
        { "M4A1", WeaponDataToJson(CreateDefaultWeaponData(
            { 1, 0, -4, 4, -6, -4, 14, 8, 18, -4, -14, -25, -19, -22, 1, 8, -9, -13, 3, 1 },
            { 6, 4, 14, 18, 21, 24, 14, 12, 5, 10, 5, -3, 0, -3, 3, 3, 1, -2, 2, 1 },
            { 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 215 }
        )) },
        { "M4A4", WeaponDataToJson(CreateDefaultWeaponData(
            { 0, 2, 0, -6, 7, -9, -5, 16, 11, 22, -4, -18, -30, -24, -25, 0, 8, -11, -13, 2, 33, 10, 27, 10, 11, -12, 6, 4, 3, 4 },
            { 0, 7, 9, 16, 21, 23, 27, 15, 13, 5, 11, 6, -4, 0, -6, 4, 4, 1, -2, 2, -1, 6, 3, 2, 0, 0, 5, 5, 1, -1 },
            { 15, 88, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 215 }
        )) },
        { "AK47", WeaponDataToJson(CreateDefaultWeaponData(
            { -4, 4, -3, -1, 13, 8, 13, -17, -42, -21, 12, -15, -26, -3, 40, 19, 14, 27, 33, -21, 7, -7, -8, 19, 5, -20, -33, -45, -14, -14 },
            { 7, 19, 29, 31, 31, 28, 21, 12, -3, 2, 11, 7, -8, 4, 1, 7, 10, 0, -10, -2, 3, 9, 4, -3, 6, -1, -4, -21, 1, 1 },
            { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 }
        )) },
        { "GALIL", WeaponDataToJson(CreateDefaultWeaponData(
            { 4, -2, 6, 12, -1, 2, 6, 11, -4, -22, -30, -29, -9, -12, -7, 0, 4, 25, 14, 25, 31, 6, -12, 13, 10, 16, -9, -32, -24, -15, 6, -14, -24, -13, -13 },
            { 4, 5, 10, 15, 21, 24, 16, 10, 14, 8, -3, -13, 8, 2, 1, 1, 7, 7, 4, -3, -9, 3, 3, -1, -1, -4, 5, -5, -3, 5, 8, -3, -14, -1, -1 },
            { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 50, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90 }
        )) },
        { "FAMAS", WeaponDataToJson(CreateDefaultWeaponData(
            { -4, 1, -6, -1, 0, 14, 16, -6, -20, -16, -13, 4, 23, 12, 20, 5, 15, 3, -4, -25, -3, 11, 15, 15, 15 },
            { 5, 4, 10, 17, 20, 18, 12, 12, 8, 5, 2, 5, 4, 6, -3, 0, 0, 5, 3, -1, 2, 0, -7, -10, -10 },
            { 30, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 87, 88, 88, 88, 88, 88, 80, 88, 80, 84, 80, 88, 215 }
        )) },
        { "AUG", WeaponDataToJson(CreateDefaultWeaponData(
            { 5, 0, -5, -7, 15, 6, 10, 12, 14, -18, -20, 9, 10, -22, -10, -20, -8, -10, -9, 15, 18, 15, -5, 15, 15, 15, -16, -19, -14, -8 },
            { 6, 13, 22, 26, 7, 30, 21, 15, 13, 11, 6, 0, 6, 5, -11, -13, 6, 5, 0, 1, 3, 6, 1, -3, -11, 0, 6, 3, 1, 1 },
            { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 }
        )) },
        { "SG", WeaponDataToJson(CreateDefaultWeaponData(
            { -4, -13, -9, -6, -8, -8, -6, -5, 7, 6, -8, -15, -5, -6, -8, -2, -18, -20, -10, 19, 20, 15, 20, 15, 20, 20, 20, 20, 16, 6 },
            { 9, 15, 25, 29, 31, 36, 8, 7, 9, 9, 12, 8, 5, 5, 6, 6, -6, -13, -9, -2, 3, -5, -1, -1, -1, -1, 9, 6, 5, 5 },
            { 99, 99, 99, 99, 99, 99, 45, 45, 45, 45, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 45, 45, 45, 45, 99, 99, 99, 215 }
        )) }
    };
}

const json Config::DEFAULT_CONFIG =
{
    { "weaponKeys", {
        { "OFF", 0x7B },
        { "UMP", 0x70 },
        { "M4A1", 0x71 },
        { "M4A4", 0x72 },
        { "AK47", 0x73 },
        { "GALIL", 0x74 },
        { "FAMAS", 0x75 },
        { "AUG", 0x76 },
        { "SG", 0x77 }
    }},

    { "bhop", false },
    { "rapidFire", true },
    { "fastReload", false },

    { "sensitivity", 8.0 },
    { "zoomSensitivity", 1.0 },
    { "colorThreshold", 20 },

    { "confirmationKey", 0x00 },
    { "colorBotKey", 0x58 },
    { "autoAcceptKey", 0x48 },
    { "turnAroundKey", 0x5A },
    { "turnAroundDistance", 1022 },
};

Config::Config()
{
    if (!Validate(DEFAULT_CONFIG))
    {
        throw runtime_error("Invalid default config");
    }

    FromJson(DEFAULT_CONFIG);
    
    json defaultConfigWithWeapons = DEFAULT_CONFIG;
    defaultConfigWithWeapons["weaponRecoilData"] = CreateDefaultWeaponRecoilData();
    LoadWeaponRecoilData(defaultConfigWithWeapons);
    
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
    for (const auto* key : REQUIRED_KEYS)
    {
        if (!j.contains(key))
        {
            std::cerr << "[!] Missing required key: " << key << std::endl;
            return false;
        }
    }
    
    if (j.contains("weaponKeys") && j["weaponKeys"].is_object())
    {
        for (const auto& info : WEAPONS)
        {
            if (!j["weaponKeys"].contains(info.id))
            {
                std::cerr << "[!] Missing weapon key: " << info.id << std::endl;
                return false;
            }
        }
    }

    auto TypeToString = [](json::value_t type) -> std::string
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

        for (const auto& check : TYPE_CHECKS)
        {
            if (j[check.key].type() != check.type)
            {
                std::cerr << "[!] Invalid type for '" << check.key << "'. Expected: " << TypeToString(check.type) << std::endl;
                valid = false;
            }
        }

        if (j.contains("weaponKeys") && j["weaponKeys"].is_object())
        {
            for (auto it = j["weaponKeys"].begin(); it != j["weaponKeys"].end(); ++it)
            {
                const double value = it.value().get<double>();
                if (value < 0 || value > 0xFE)
                {
                    std::cerr << "[!] Invalid weapon key value for " << it.key() << ": " << value << " (valid range: 0-0xFE)" << std::endl;
                    valid = false;
                }
            }
        }

        for (const auto& range : RANGE_CHECKS)
        {
            const double value = j[range.key].get<double>();

            if (value < range.min || value > range.max)
            {
                std::cerr << "[!] Invalid value for " << range.key << ": " << value
                    << " (valid range: " << range.min << "-" << range.max << ")" << std::endl;
                valid = false;
            }
        }

        return valid;
    }
    catch (const json::exception& e)
    {
        std::cerr << "[!] JSON parsing error: " << e.what() << std::endl;
        return false;
    }
}

std::string Config::GenerateHotkeysString() const
{
    std::string result;

    for (const auto& info : WEAPONS)
    {
        if (!result.empty()) result += " | ";
        result += "[" + KeyCodeToString(GetWeaponKey(info.weapon)) + "] - " + string(info.display);
    }

    return result;
}

void Config::InteractiveSetup()
{
    ConfigWizard(*this).Run();
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

std::string Config::KeyCodeToString(int code) const
{
    if (code == 0) return "NONE";

    char name[128] = { 0 };
    UINT scanCode = MapVirtualKey(code, MAPVK_VK_TO_VSC);

    LONG lParam = scanCode << 16;

    if (code == VK_RIGHT || code == VK_LEFT || code == VK_UP || code == VK_DOWN || code == VK_INSERT || code == VK_DELETE || code == VK_HOME || code == VK_END || code == VK_PRIOR || code == VK_NEXT)
    {
        lParam |= 0x01000000;
    }

    if (GetKeyNameTextA(lParam, name, sizeof(name)) != 0)
    {
        return string(name);
    }

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
    printLine("Turn Around", KeyCodeToString(turnAroundKey));
    printLine("Turn Around Distance", to_string(turnAroundDistance));

    cout << endl;
    for (const auto& info : WEAPONS)
    {
        printLine(info.display, KeyCodeToString(GetWeaponKey(info.weapon)));
    }

    cout << "---------------------------" << endl;
}

json Config::ToJson() const
{
    json result = {
        { "bhop", bhop },
        { "rapidFire", rapidFire },
        { "fastReload", fastReload },

        { "sensitivity", sensitivity },
        { "zoomSensitivity", zoomSensitivity },
        { "colorThreshold", colorThreshold },

        { "confirmationKey", confirmationKey },
        { "colorBotKey", colorBotKey },
        { "autoAcceptKey", autoAcceptKey },

        { "turnAroundKey", turnAroundKey },
        { "turnAroundDistance", turnAroundDistance }
    };

    json weaponKeysJson;
    for (const auto& [weapon, key] : weaponKeys)
    {
        string weaponName = WeaponToString(weapon);
        if (!weaponName.empty())
        {
            weaponKeysJson[weaponName] = key;
        }
    }
    result["weaponKeys"] = weaponKeysJson;

    json recoilData;
    for (const auto& [weapon, data] : weaponRecoilData)
    {
        string weaponName = WeaponToString(weapon);
        if (!weaponName.empty())
        {
            recoilData[weaponName] = WeaponDataToJson(data);
        }
    }

    result["weaponRecoilData"] = recoilData;
    return result;
}

void Config::LoadWeaponRecoilData(const json& j)
{
    weaponRecoilData.clear();

    if (j.contains("weaponRecoilData") && j["weaponRecoilData"].is_object())
    {
        const auto& recoilData = j["weaponRecoilData"];
        for (auto it = recoilData.begin(); it != recoilData.end(); ++it)
        {
            Weapon weapon = WeaponFromString(it.key());
            if (weapon != OFF)
            {
                weaponRecoilData[weapon] = WeaponDataFromJson(it.value());
            }
        }
    }
    else
    {
        json defaultConfigWithWeapons = DEFAULT_CONFIG;
        defaultConfigWithWeapons["weaponRecoilData"] = CreateDefaultWeaponRecoilData();
        LoadWeaponRecoilData(defaultConfigWithWeapons);
    }
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

    readKey("turnAroundKey", turnAroundKey);
    readKey("turnAroundDistance", turnAroundDistance);

    weaponKeys.clear();
    if (j.contains("weaponKeys") && j["weaponKeys"].is_object())
    {
        for (auto it = j["weaponKeys"].begin(); it != j["weaponKeys"].end(); ++it)
        {
            Weapon weapon = WeaponFromString(it.key());
            if (weapon != OFF || it.key() == "OFF")
            {
                weaponKeys[weapon] = it.value().get<int>();
            }
        }
    }
    else
    {
        const vector<pair<string, Weapon>> oldFormatKeys =
        {
            { "umpKey", UMP },
            { "m4a1Key", M4A1 },
            { "m4a4Key", M4A4 },
            { "ak47Key", AK47 },
            { "galilKey", GALIL },
            { "famasKey", FAMAS },
            { "augKey", AUG },
            { "sgKey", SG },
            { "offKey", OFF }
        };
        
        for (const auto& [keyName, weapon] : oldFormatKeys)
        {
            if (j.contains(keyName))
            {
                weaponKeys[weapon] = j[keyName].get<int>();
            }
            else if (DEFAULT_CONFIG.contains("weaponKeys") && DEFAULT_CONFIG["weaponKeys"].contains(WeaponToString(weapon)))
            {
                weaponKeys[weapon] = DEFAULT_CONFIG["weaponKeys"][WeaponToString(weapon)].get<int>();
            }
        }
    }

    LoadWeaponRecoilData(j);
}

string Config::FormatFloat(double value, int precision)
{
    ostringstream oss;
    oss << fixed << setprecision(precision) << value;
    return oss.str();
}

WeaponData Config::GetWeaponRecoilData(Weapon weapon) const
{
    auto it = weaponRecoilData.find(weapon);
    if (it != weaponRecoilData.end())
    {
        return it->second;
    }
    return {};
}

void Config::Save() const
{
    ofstream file("Config.json");
    file << setw(4) << ToJson() << endl;
}