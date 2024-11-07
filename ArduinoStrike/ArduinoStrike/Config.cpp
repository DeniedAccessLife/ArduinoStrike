#include "Config.h"

Config::Config()
{
    Load();
}

void Config::Load()
{
    ifstream file("Settings.cfg");

    if (!file.is_open())
    {
        int bhop = GetValidatedIntInput("Enter bhop boolean value (1/0) -> ", 0, 1);
        SetBhop(bhop);

        int rapidFire = GetValidatedIntInput("Enter rapid-fire boolean value (1/0) -> ", 0, 1);
        SetRapidFire(rapidFire);

        int sensitivity = GetValidatedIntInput("Enter sensitivity integer value (1-8) -> ", 1, 8);
        SetSensitivity(sensitivity);

        double zoomSensitivity = GetValidatedDoubleInput("Enter zoom sensitivity double value (0.01-3.00) -> ", 0.01, 3.00);
        SetZoomSensitivity(zoomSensitivity);

        int confirmationKey = GetValidatedKeyInput("Enter recoil control confirmation key (0/VK_CODE) -> ");
        SetConfirmationKey(confirmationKey);

        int colorBotKey = GetValidatedKeyInput("Enter colorbot activation key (0/VK_CODE) -> ");
        SetColorBotKey(colorBotKey);

        int colorBotThreshold = GetValidatedIntInput("Enter colorbot color deviation (0-20) -> ", 0, 20);
        SetColorBotThreshold(colorBotThreshold);

        Save();
        cout << "Configuration successfully saved!" << endl;
    }
    else
    {
        file >> dec >> bhop;
        file >> dec >> rapidFire;
        file >> dec >> sensitivity;
        file >> dec >> zoomSensitivity;
        file >> hex >> confirmationKey;
        file >> hex >> colorBotKey;
        file >> dec >> colorBotThreshold;
        file.close();

        if (!Validate())
        {
            cerr << "Invalid values in config file. Please enter valid values!" << endl;
            remove("Settings.cfg");
            exit(1);
        }

        cout << "Configuration successfully loaded!" << endl;
    }
}

void Config::Save() const
{
    ofstream out("Settings.cfg");
    out << dec << bhop << endl;
    out << dec << rapidFire << endl;
    out << dec << sensitivity << endl;
    out << dec << zoomSensitivity << endl;
    out << hex << confirmationKey << endl;
    out << hex << colorBotKey << endl;
    out << dec << colorBotThreshold;
    out.close();
}

bool Config::Validate() const
{
    return (bhop == 0 || bhop == 1) && (rapidFire == 0 || rapidFire == 1) && (sensitivity >= 1 && sensitivity <= 8) && (confirmationKey == 0 || (confirmationKey >= 0x01 && confirmationKey <= 0xFE)) && (colorBotKey == 0 || (colorBotKey >= 0x01 && colorBotKey <= 0xFE)) && (colorBotThreshold >= 0 && colorBotThreshold <= 20) && (zoomSensitivity >= 0.01 && zoomSensitivity <= 3.00);
}

int Config::GetBhop() const
{
    return bhop;
}

void Config::SetBhop(int value)
{
    bhop = value;
}

int Config::GetRapidFire() const
{
    return rapidFire;
}

void Config::SetRapidFire(int value)
{
    rapidFire = value;
}

int Config::GetSensitivity() const
{
    return sensitivity;
}

void Config::SetSensitivity(int value)
{
    sensitivity = value;
}

double Config::GetZoomSensitivity() const
{
    return zoomSensitivity;
}

void Config::SetZoomSensitivity(double value)
{
    zoomSensitivity = value;
}

int Config::GetConfirmationKey() const
{
    return confirmationKey;
}

void Config::SetConfirmationKey(int value)
{
    confirmationKey = value;
}

int Config::GetColorBotKey() const
{
    return colorBotKey;
}

void Config::SetColorBotKey(int value)
{
    colorBotKey = value;
}

int Config::GetColorBotThreshold() const
{
    return colorBotThreshold;
}

void Config::SetColorBotThreshold(int value)
{
    colorBotThreshold = value;
}

int Config::GetValidatedKeyInput(const string& prompt)
{
    string input;
    int value = -1;

    do
    {
        cout << prompt;
        getline(cin, input);
        istringstream iss(input);

        if (!(iss >> hex >> value) || (value != 0 && (value < 0x01 || value > 0xFE)))
        {
            value = -1;
            cout << "Invalid input! Please enter 0 or VK_CODE." << endl << endl;
        }
    }
    while (value == -1);

    return value;
}

int Config::GetValidatedIntInput(const string& prompt, int min, int max)
{
    string input;
    int value = -1;

    do
    {
        cout << prompt;
        getline(cin, input);
        istringstream iss(input);

        if (!(iss >> dec >> value) || value < min || value > max)
        {
            value = -1;

            if (min == 0 && max == 1)
            {
                cout << "Invalid input! Please enter 0 or 1." << endl << endl;
            }
            else
            {
                cout << "Invalid input! Please enter an integer between " << min << " and " << max << "." << endl << endl;
            }
        }
    }
    while (value == -1);

    return value;
}

double Config::GetValidatedDoubleInput(const string& prompt, double min, double max)
{
    string input;
    double value = -1.0;

    do
    {
        cout << prompt;
        getline(cin, input);
        istringstream iss(input);

        if (!(iss >> dec >> value) || value < min || value > max)
        {
            value = -1.0;
            cout << "Invalid input! Please enter a number between " << fixed << setprecision(2) << min << " and " << max << "." << endl << endl;
        }
    }
    while (value == -1.0);

    return value;
}