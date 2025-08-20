#pragma once

#include "pch.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class Config {
public:
    Config();
    void Load();
    void Save() const;
    bool Validate(const json& j) const;
    string GenerateHotkeysString() const;

    int GetUMPKey() const { return umpKey; }
    int GetM4A1Key() const { return m4a1Key; }
    int GetM4A4Key() const { return m4a4Key; }
    int GetAK47Key() const { return ak47Key; }
    int GetGALILKey() const { return galilKey; }
    int GetFAMASKey() const { return famasKey; }
    int GetAUGKey() const { return augKey; }
    int GetSGKey() const { return sgKey; }
    int GetOFFKey() const { return offKey; }

    bool GetBhop() const { return bhop; }
    bool GetRapidFire() const { return rapidFire; }
    double GetSensitivity() const { return sensitivity; }
    double GetZoomSensitivity() const { return zoomSensitivity; }
    int GetConfirmationKey() const { return confirmationKey; }
    int GetColorBotKey() const { return colorBotKey; }
    int GetColorThreshold() const { return colorThreshold; }
    bool GetFastReload() const { return fastReload; }
    int GetAutoAcceptKey() const { return autoAcceptKey; }

private:
    int umpKey = 0x00;
    int m4a1Key = 0x00;
    int m4a4Key = 0x00;
    int ak47Key = 0x00;
    int galilKey = 0x00;
    int famasKey = 0x00;
    int augKey = 0x00;
    int sgKey = 0x00;
    int offKey = 0x00;

    bool bhop;
    bool rapidFire;
    bool fastReload;

    double sensitivity;
    double zoomSensitivity;
    int colorThreshold;

    int confirmationKey = 0x00;
    int colorBotKey = 0x00;
    int autoAcceptKey = 0x00;

    void InteractiveSetup();
    bool OfferDefaultConfig();
    void ConfigureFeatures();
    void ConfigureSettings();
    void ConfigureKeys();
    int WaitForKeyPress() const;
    string KeyCodeToString(int code) const;
    void PrintSuccess() const;

    json ToJson() const;
    void FromJson(const json& j);
    
    static string FormatFloat(double value, int precision = 2);

    static const json DEFAULT_CONFIG;
};