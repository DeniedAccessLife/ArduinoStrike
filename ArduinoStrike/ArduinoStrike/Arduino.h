#pragma once
#pragma comment(lib, "Setupapi.lib")

#include <regex>
#include <thread>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <devguid.h>
#include <SetupAPI.h>

#ifdef _DEBUG
#define LOG(msg) Arduino::LogMessage(msg)
#else
#define LOG(msg)
#endif

using namespace std;
using namespace chrono;
using namespace this_thread;

struct DeviceInfo
{
    string port;
    string hardware_id;
    string friendly_name;
};

class Arduino
{
public:
    ~Arduino();
    Arduino(LPCSTR name);
    bool WriteMessage(const string& message) const;

private:
    HANDLE handle;
    bool GetDevice(LPCSTR name, LPSTR port);
    bool LoadConfiguration(const string& file_name, DeviceInfo& config_device);
    bool ExtractProperties(HDEVINFO device_info, SP_DEVINFO_DATA& dev_info_data, DeviceInfo& device);
    bool SelectDevice(const vector<DeviceInfo>& devices, LPSTR port);
    static void LogMessage(const string& message);
};