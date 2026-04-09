#pragma once

#include "pch.h"
#include "json.hpp"

#include <string>
#include <unordered_map>

using json = nlohmann::json;

#include "Weapons.h"

class Config {
public:
    Config();
    void Load();
    void Save() const;
    bool Validate(const json& j) const;
    std::string GenerateHotkeysString() const;

    int GetWeaponKey(Weapon weapon) const;
    
    int GetUMPKey() const;
    int GetM4A1Key() const;
    int GetM4A4Key() const;
    int GetAK47Key() const;
    int GetGALILKey() const;
    int GetFAMASKey() const;
    int GetAUGKey() const;
    int GetSGKey() const;
    int GetOFFKey() const;

    int GetConfirmationKey() const { return confirmationKey; }
    int GetColorBotKey() const { return colorBotKey; }
    int GetAutoAcceptKey() const { return autoAcceptKey; }
    int GetTurnAroundKey() const { return turnAroundKey; }
    int GetTurnAroundDistance() const { return turnAroundDistance; }

    bool GetBhop() const { return bhop; }
    bool GetRapidFire() const { return rapidFire; }
    bool GetFastReload() const { return fastReload; }
    double GetSensitivity() const { return sensitivity; }
    double GetZoomSensitivity() const { return zoomSensitivity; }
    int GetColorThreshold() const { return colorThreshold; }

    WeaponData GetWeaponRecoilData(Weapon weapon) const;

private:
    std::unordered_map<Weapon, int> weaponKeys;

    int confirmationKey = 0x00;
    int colorBotKey = 0x00;
    int autoAcceptKey = 0x00;
    int turnAroundKey = 0x00;
    int turnAroundDistance = 1800;

    bool bhop;
    bool rapidFire;
    bool fastReload;
    double sensitivity;
    double zoomSensitivity;
    int colorThreshold;

    std::unordered_map<Weapon, WeaponData> weaponRecoilData;
    
    static std::string WeaponToString(Weapon weapon);
    static Weapon WeaponFromString(const std::string& name);

    void InteractiveSetup();

    int WaitForKeyPress() const;
    std::string KeyCodeToString(int code) const;
    void PrintSuccess() const;

    json ToJson() const;
    void FromJson(const json& j);
    void LoadWeaponRecoilData(const json& j);
    
    static std::string FormatFloat(double value, int precision = 2);

    static const json DEFAULT_CONFIG;

    friend class ConfigWizard;
};