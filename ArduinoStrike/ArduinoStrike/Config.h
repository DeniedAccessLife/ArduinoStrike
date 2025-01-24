#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>
#include <unordered_map>

using namespace std;
using namespace filesystem;

/**
 * @brief This class handles the loading, saving, and validation
 *        of user configurations from/to "Settings.cfg". It allows
 *        users to input key names for various features and weapon
 *        hotkeys. If a user leaves a weapon hotkey input blank,
 *        that weapon's hotkey will be disabled.
 */
class Config
{
public:
    Config();

    // Main methods to load/save and validate
    void Load();
    void Save() const;
    bool Validate() const;

    // Basic features
    int GetBhop() const;
    void SetBhop(int value);

    int GetRapidFire() const;
    void SetRapidFire(int value);

    int GetSensitivity() const;
    void SetSensitivity(int value);

    double GetZoomSensitivity() const;
    void SetZoomSensitivity(double value);

    int GetConfirmationKey() const;
    void SetConfirmationKey(int value);

    int GetColorBotKey() const;
    void SetColorBotKey(int value);

    int GetColorBotThreshold() const;
    void SetColorBotThreshold(int value);

    int GetFastReload() const;
    void SetFastReload(int value);

    // Weapon hotkeys
    int  GetKeyOff() const;
    void SetKeyOff(int value);

    int  GetKeyUMP() const;
    void SetKeyUMP(int value);

    int  GetKeyM4A1() const;
    void SetKeyM4A1(int value);

    int  GetKeyM4A4() const;
    void SetKeyM4A4(int value);

    int  GetKeyAK47() const;
    void SetKeyAK47(int value);

    int  GetKeyGALIL() const;
    void SetKeyGALIL(int value);

    int  GetKeyFAMAS() const;
    void SetKeyFAMAS(int value);

    int  GetKeyAUG() const;
    void SetKeyAUG(int value);

    int  GetKeySG() const;
    void SetKeySG(int value);

    int  GetKeyBIZON() const;
    void SetKeyBIZON(int value);

    int  GetKeyCZ75() const;
    void SetKeyCZ75(int value);

    int  GetKeyM249() const;
    void SetKeyM249(int value);

    int  GetKeyMAC10() const;
    void SetKeyMAC10(int value);

    int  GetKeyMP5SD() const;
    void SetKeyMP5SD(int value);

    int  GetKeyMP7() const;
    void SetKeyMP7(int value);

    int  GetKeyMP9() const;
    void SetKeyMP9(int value);

    int  GetKeyP90() const;
    void SetKeyP90(int value);

private:
    // Stored config values
    int bhop;
    int rapidFire;
    int sensitivity;
    double zoomSensitivity;
    int confirmationKey;
    int colorBotKey;
    int colorBotThreshold;
    int fastReload;

    // Custom weapon hotkeys
    int keyOff;
    int keyUMP;
    int keyM4A1;
    int keyM4A4;
    int keyAK47;
    int keyGALIL;
    int keyFAMAS;
    int keyAUG;
    int keySG;
    int keyBIZON;
    int keyCZ75;
    int keyM249;
    int keyMAC10;
    int keyMP5SD;
    int keyMP7;
    int keyMP9;
    int keyP90;

    // Internal validation helpers
    int    GetValidatedIntInput(const string& prompt, int min, int max);
    double GetValidatedDoubleInput(const string& prompt, double min, double max);
    int    GetValidatedKeyInput(const string& prompt);

    // Helper to convert key names to VK codes
    int    ConvertKeyNameToVKCode(const string& keyName) const;
};
