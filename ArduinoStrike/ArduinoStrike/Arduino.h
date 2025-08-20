#pragma once

#pragma comment(lib, "Setupapi.lib")
#include "Utils.h"
#include <devguid.h>
#include <SetupAPI.h>
using namespace std;
using namespace std::chrono;
using namespace this_thread;
using namespace boost::asio;

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
    Arduino(LPCSTR name, bool dry_run = false);
    bool WriteMessage(const string& message);

private:
    bool dry_run;
    io_context io_context;
    serial_port serial_port;

    bool GetDevice(LPCSTR name, LPSTR port);
    bool LoadConfiguration(const string& file_name, DeviceInfo& config_device);
    bool ExtractProperties(HDEVINFO device_info, SP_DEVINFO_DATA& dev_info_data, DeviceInfo& device);
    bool SelectDevice(const vector<DeviceInfo>& devices, LPSTR port);
};