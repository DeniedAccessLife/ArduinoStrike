#include "Config.h"

/**
 * @brief Constructor automatically loads the configuration
 *        from file. If the file doesn't exist, it prompts
 *        the user for input and creates a new file.
 */
Config::Config()
{
    Load();
}

/**
 * @brief Attempts to open "Settings.cfg". If it exists,
 *        reads the data from file. Otherwise prompts
 *        the user for input and writes to a new file.
 */
void Config::Load()
{
    ifstream file("Settings.cfg");

    // If file doesn't exist or fail to open, prompt user to create one
    if (!file.is_open())
    {
        // Basic configs
        int bhopVal = GetValidatedIntInput("Enter bhop boolean value (1/0) -> ", 0, 1);
        SetBhop(bhopVal);

        int rapidFireVal = GetValidatedIntInput("Enter rapid-fire boolean value (1/0) -> ", 0, 1);
        SetRapidFire(rapidFireVal);

        int sensitivityVal = GetValidatedIntInput("Enter sensitivity integer value (1-8) -> ", 1, 8);
        SetSensitivity(sensitivityVal);

        double zoomVal = GetValidatedDoubleInput("Enter zoom sensitivity (0.01-3.00) -> ", 0.01, 3.00);
        SetZoomSensitivity(zoomVal);

        int confirmKey = GetValidatedKeyInput("Enter recoil control confirmation key (e.g., A, lalt) or leave blank to disable -> ");
        SetConfirmationKey(confirmKey);

        int cbotKey = GetValidatedKeyInput("Enter colorbot activation key (e.g., A, lalt) or leave blank to disable -> ");
        SetColorBotKey(cbotKey);

        int cbotThreshold = GetValidatedIntInput("Enter colorbot color deviation (0-20) -> ", 0, 20);
        SetColorBotThreshold(cbotThreshold);

        int fastReloadVal = GetValidatedIntInput("Enter fast reload boolean value (1/0) -> ", 0, 1);
        SetFastReload(fastReloadVal);

        // Weapon hotkeys
        int offKey = GetValidatedKeyInput("Enter OFF weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyOff(offKey);

        int umpKey = GetValidatedKeyInput("Enter UMP weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyUMP(umpKey);

        int m4a1Key = GetValidatedKeyInput("Enter M4A1 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyM4A1(m4a1Key);

        int m4a4Key = GetValidatedKeyInput("Enter M4A4 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyM4A4(m4a4Key);

        int ak47Key = GetValidatedKeyInput("Enter AK47 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyAK47(ak47Key);

        int galilKey = GetValidatedKeyInput("Enter GALIL weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyGALIL(galilKey);

        int famasKey = GetValidatedKeyInput("Enter FAMAS weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyFAMAS(famasKey);

        int augKey = GetValidatedKeyInput("Enter AUG weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyAUG(augKey);

        int sgKey = GetValidatedKeyInput("Enter SG weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeySG(sgKey);

        int bizonKey = GetValidatedKeyInput("Enter BIZON weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyBIZON(bizonKey);

        int cz75Key = GetValidatedKeyInput("Enter CZ75 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyCZ75(cz75Key);

        int m249Key = GetValidatedKeyInput("Enter M249 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyM249(m249Key);

        int mac10Key = GetValidatedKeyInput("Enter MAC10 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyMAC10(mac10Key);

        int mp5sdKey = GetValidatedKeyInput("Enter MP5SD weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyMP5SD(mp5sdKey);

        int mp7Key = GetValidatedKeyInput("Enter MP7 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyMP7(mp7Key);

        int mp9Key = GetValidatedKeyInput("Enter MP9 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyMP9(mp9Key);

        int p90Key = GetValidatedKeyInput("Enter P90 weapon key (e.g., A, lalt) or leave blank to disable -> ");
        SetKeyP90(p90Key);

        // Save to file
        Save();
        cout << "Configuration successfully saved!" << endl;
    }
    else
    {
        // File found, read config
        file >> dec >> bhop;
        file >> dec >> rapidFire;
        file >> dec >> sensitivity;
        file >> dec >> zoomSensitivity;

        // Keys read as integers (VK codes)
        file >> dec >> confirmationKey;
        file >> dec >> colorBotKey;

        file >> dec >> colorBotThreshold;
        file >> dec >> fastReload;

        // Weapon hotkeys
        file >> dec >> keyOff;
        file >> dec >> keyUMP;
        file >> dec >> keyM4A1;
        file >> dec >> keyM4A4;
        file >> dec >> keyAK47;
        file >> dec >> keyGALIL;
        file >> dec >> keyFAMAS;
        file >> dec >> keyAUG;
        file >> dec >> keySG;
        file >> dec >> keyBIZON;
        file >> dec >> keyCZ75;
        file >> dec >> keyM249;
        file >> dec >> keyMAC10;
        file >> dec >> keyMP5SD;
        file >> dec >> keyMP7;
        file >> dec >> keyMP9;
        file >> dec >> keyP90;

        file.close();

        // Validate the data we read
        if (!Validate())
        {
            cerr << "Invalid values in config file. Please enter valid values!" << endl;
            remove("Settings.cfg");
            exit(1);
        }

        cout << "Configuration successfully loaded!" << endl;
    }
}

/**
 * @brief Writes all current config values to "Settings.cfg".
 *        Keys are stored as integer VK codes in decimal format.
 */
void Config::Save() const
{
    ofstream out("Settings.cfg");
    if (!out.is_open())
    {
        cerr << "Failed to open Settings.cfg for writing." << endl;
        return;
    }

    // Write numeric values in decimal
    out << dec << bhop << endl;
    out << dec << rapidFire << endl;
    out << dec << sensitivity << endl;
    out << dec << zoomSensitivity << endl;

    // Write keys as integers in decimal
    out << dec << confirmationKey << endl;
    out << dec << colorBotKey << endl;

    out << dec << colorBotThreshold << endl;
    out << dec << fastReload << endl;

    // Weapon hotkeys in decimal
    out << dec << keyOff     << endl;
    out << dec << keyUMP     << endl;
    out << dec << keyM4A1    << endl;
    out << dec << keyM4A4    << endl;
    out << dec << keyAK47    << endl;
    out << dec << keyGALIL   << endl;
    out << dec << keyFAMAS   << endl;
    out << dec << keyAUG     << endl;
    out << dec << keySG      << endl;
    out << dec << keyBIZON   << endl;
    out << dec << keyCZ75    << endl;
    out << dec << keyM249    << endl;
    out << dec << keyMAC10   << endl;
    out << dec << keyMP5SD   << endl;
    out << dec << keyMP7     << endl;
    out << dec << keyMP9     << endl;
    out << dec << keyP90     << endl;

    out.close();
}

/**
 * @brief Checks if all config values fall within valid ranges.
 * @return true if all valid, otherwise false
 */
bool Config::Validate() const
{
    bool baseValid =
        (bhop == 0 || bhop == 1) &&
        (rapidFire == 0 || rapidFire == 1) &&
        (sensitivity >= 1 && sensitivity <= 8) &&
        (zoomSensitivity >= 0.01 && zoomSensitivity <= 3.00) &&
        (fastReload == 0 || fastReload == 1) &&
        // 0 or valid VK codes
        (confirmationKey == 0 || (confirmationKey >= 0x01 && confirmationKey <= 0xFE)) &&
        (colorBotKey == 0 || (colorBotKey >= 0x01 && colorBotKey <= 0xFE));

    // Validate weapon hotkeys
    bool weaponKeyValid =
        (keyOff    == 0 || (keyOff    >= 0x01 && keyOff    <= 0xFE)) &&
        (keyUMP    == 0 || (keyUMP    >= 0x01 && keyUMP    <= 0xFE)) &&
        (keyM4A1   == 0 || (keyM4A1   >= 0x01 && keyM4A1   <= 0xFE)) &&
        (keyM4A4   == 0 || (keyM4A4   >= 0x01 && keyM4A4   <= 0xFE)) &&
        (keyAK47   == 0 || (keyAK47   >= 0x01 && keyAK47   <= 0xFE)) &&
        (keyGALIL  == 0 || (keyGALIL  >= 0x01 && keyGALIL  <= 0xFE)) &&
        (keyFAMAS  == 0 || (keyFAMAS  >= 0x01 && keyFAMAS  <= 0xFE)) &&
        (keyAUG    == 0 || (keyAUG    >= 0x01 && keyAUG    <= 0xFE)) &&
        (keySG     == 0 || (keySG     >= 0x01 && keySG     <= 0xFE)) &&
        (keyBIZON  == 0 || (keyBIZON  >= 0x01 && keyBIZON  <= 0xFE)) &&
        (keyCZ75   == 0 || (keyCZ75   >= 0x01 && keyCZ75   <= 0xFE)) &&
        (keyM249   == 0 || (keyM249   >= 0x01 && keyM249   <= 0xFE)) &&
        (keyMAC10  == 0 || (keyMAC10  >= 0x01 && keyMAC10  <= 0xFE)) &&
        (keyMP5SD  == 0 || (keyMP5SD  >= 0x01 && keyMP5SD  <= 0xFE)) &&
        (keyMP7    == 0 || (keyMP7    >= 0x01 && keyMP7    <= 0xFE)) &&
        (keyMP9    == 0 || (keyMP9    >= 0x01 && keyMP9    <= 0xFE)) &&
        (keyP90    == 0 || (keyP90    >= 0x01 && keyP90    <= 0xFE));

    return baseValid && weaponKeyValid;
}

// ----------------------- Getters / Setters -----------------------
int Config::GetBhop() const { return bhop; }
void Config::SetBhop(int value) { bhop = value; }

int Config::GetRapidFire() const { return rapidFire; }
void Config::SetRapidFire(int value) { rapidFire = value; }

int Config::GetSensitivity() const { return sensitivity; }
void Config::SetSensitivity(int value) { sensitivity = value; }

double Config::GetZoomSensitivity() const { return zoomSensitivity; }
void Config::SetZoomSensitivity(double value) { zoomSensitivity = value; }

int Config::GetConfirmationKey() const { return confirmationKey; }
void Config::SetConfirmationKey(int value) { confirmationKey = value; }

int Config::GetColorBotKey() const { return colorBotKey; }
void Config::SetColorBotKey(int value) { colorBotKey = value; }

int Config::GetColorBotThreshold() const { return colorBotThreshold; }
void Config::SetColorBotThreshold(int value) { colorBotThreshold = value; }

int Config::GetFastReload() const { return fastReload; }
void Config::SetFastReload(int value) { fastReload = value; }

// --- Weapon hotkeys ---
int Config::GetKeyOff() const { return keyOff; }
void Config::SetKeyOff(int value) { keyOff = value; }

int Config::GetKeyUMP() const { return keyUMP; }
void Config::SetKeyUMP(int value) { keyUMP = value; }

int Config::GetKeyM4A1() const { return keyM4A1; }
void Config::SetKeyM4A1(int value) { keyM4A1 = value; }

int Config::GetKeyM4A4() const { return keyM4A4; }
void Config::SetKeyM4A4(int value) { keyM4A4 = value; }

int Config::GetKeyAK47() const { return keyAK47; }
void Config::SetKeyAK47(int value) { keyAK47 = value; }

int Config::GetKeyGALIL() const { return keyGALIL; }
void Config::SetKeyGALIL(int value) { keyGALIL = value; }

int Config::GetKeyFAMAS() const { return keyFAMAS; }
void Config::SetKeyFAMAS(int value) { keyFAMAS = value; }

int Config::GetKeyAUG() const { return keyAUG; }
void Config::SetKeyAUG(int value) { keyAUG = value; }

int Config::GetKeySG() const { return keySG; }
void Config::SetKeySG(int value) { keySG = value; }

int Config::GetKeyBIZON() const { return keyBIZON; }
void Config::SetKeyBIZON(int value) { keyBIZON = value; }

int Config::GetKeyCZ75() const { return keyCZ75; }
void Config::SetKeyCZ75(int value) { keyCZ75 = value; }

int Config::GetKeyM249() const { return keyM249; }
void Config::SetKeyM249(int value) { keyM249 = value; }

int Config::GetKeyMAC10() const { return keyMAC10; }
void Config::SetKeyMAC10(int value) { keyMAC10 = value; }

int Config::GetKeyMP5SD() const { return keyMP5SD; }
void Config::SetKeyMP5SD(int value) { keyMP5SD = value; }

int Config::GetKeyMP7() const { return keyMP7; }
void Config::SetKeyMP7(int value) { keyMP7 = value; }

int Config::GetKeyMP9() const { return keyMP9; }
void Config::SetKeyMP9(int value) { keyMP9 = value; }

int Config::GetKeyP90() const { return keyP90; }
void Config::SetKeyP90(int value) { keyP90 = value; }

// ------------------- User Input Validation Helpers -------------------
/**
 * @brief Prompts the user to input a key name, converts it to a VK_CODE.
 *        If the user inputs an empty string, the key is disabled (set to 0).
 *
 * @param prompt The prompt message to display.
 * @return The corresponding VK_CODE, or 0 if disabled.
 */
int Config::GetValidatedKeyInput(const string& prompt)
{
    string input;
    int value = 0; // Default to 0 (disabled)

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (input.empty())
        {
            // User chose to disable the key
            return 0;
        }

        // Convert key name to VK_CODE
        value = ConvertKeyNameToVKCode(input);

        if (value != -1)
        {
            return value;
        }
        else
        {
            cout << "Invalid key name! Please enter a valid key (e.g., A, lalt, 1, 2, 3)." << endl << endl;
        }
    }
}

/**
 * @brief Prompts the user until they input a valid integer within [min, max].
 */
int Config::GetValidatedIntInput(const string& prompt, int min, int max)
{
    string input;
    int value = -1;

    while (true)
    {
        cout << prompt;
        getline(cin, input);
        istringstream iss(input);

        if (!(iss >> dec >> value) || value < min || value > max)
        {
            cout << "Invalid input! Please enter an integer between "
                 << min << " and " << max << "." << endl << endl;
            continue;
        }

        return value;
    }
}

/**
 * @brief Prompts the user until they input a valid double within [min, max].
 */
double Config::GetValidatedDoubleInput(const string& prompt, double min, double max)
{
    string input;
    double value = -1.0;

    while (true)
    {
        cout << prompt;
        getline(cin, input);
        istringstream iss(input);

        if (!(iss >> dec >> value) || value < min || value > max)
        {
            cout << "Invalid input! Please enter a decimal number between "
                 << fixed << setprecision(2) << min << " and " << max << "." << endl << endl;
            continue;
        }

        return value;
    }
}

/**
 * @brief Converts a key name string to its corresponding VK_CODE.
 *        Supports single characters and some special keys like "lalt", "rctrl", etc.
 *
 * @param keyName The name of the key (e.g., "A", "a", "1", "lalt").
 * @return The corresponding VK_CODE, or -1 if invalid.
 */
int Config::ConvertKeyNameToVKCode(const string& keyName) const
{
    // Map of special key names to VK codes
    static unordered_map<string, int> specialKeys = {
        {"lalt", VK_LMENU},
        {"ralt", VK_RMENU},
        {"lctrl", VK_LCONTROL},
        {"rctrl", VK_RCONTROL},
        {"lshift", VK_LSHIFT},
        {"rshift", VK_RSHIFT},
        {"esc", VK_ESCAPE},
        {"space", VK_SPACE},
        {"enter", VK_RETURN},
        {"tab", VK_TAB},
        {"backspace", VK_BACK},
        {"capslock", VK_CAPITAL},
        {"numlock", VK_NUMLOCK},
        {"scrolllock", VK_SCROLL},
        {"pgup", VK_PRIOR},
        {"pgdn", VK_NEXT},
        {"home", VK_HOME},
        {"end", VK_END},
        {"insert", VK_INSERT},
        {"delete", VK_DELETE},
        {"up", VK_UP},
        {"down", VK_DOWN},
        {"left", VK_LEFT},
        {"right", VK_RIGHT},
        {"f1", VK_F1},
        {"f2", VK_F2},
        {"f3", VK_F3},
        {"f4", VK_F4},
        {"f5", VK_F5},
        {"f6", VK_F6},
        {"f7", VK_F7},
        {"f8", VK_F8},
        {"f9", VK_F9},
        {"f10", VK_F10},
        {"f11", VK_F11},
        {"f12", VK_F12}
        // Add more special keys as needed
    };

    string key = keyName;
    // Convert to lowercase for case-insensitive comparison
    for (auto & c: key) c = tolower(c);

    // Check if it's a special key
    if (specialKeys.find(key) != specialKeys.end())
    {
        return specialKeys[key];
    }

    // If it's a single character, return its VK_CODE
    if (key.length() == 1)
    {
        char c = key[0];
        // If it's a digit or letter
        if (isdigit(c) || isalpha(c))
        {
            return VkKeyScanA(c) & 0xFF;
        }
    }

    // Invalid key name
    return -1;
}
