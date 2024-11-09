#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

class Config
{
public:
    Config();

    void Load();
    void Save() const;
    bool Validate() const;

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

private:
    int bhop;
    int rapidFire;
    int sensitivity;
    double zoomSensitivity;
    int confirmationKey;

    int colorBotKey;
    int colorBotThreshold;
    int fastReload;

    int GetValidatedKeyInput(const string& prompt);
    int GetValidatedIntInput(const string& prompt, int min, int max);
    double GetValidatedDoubleInput(const string& prompt, double min, double max);
};