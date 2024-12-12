#include "Arduino.h"
#include "Utils.h"

Arduino::~Arduino()
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        LOG("Disconnected from Arduino.");
    }
}

Arduino::Arduino(LPCSTR name) : handle(INVALID_HANDLE_VALUE)
{
    char port[100] = "\\.\\";

    LOG("Searching for device...");
    while (!GetDevice(name, port))
    {
        LOG("Device not found. Retrying...");
        sleep_for(milliseconds(1000));
    }

    LOG(string("Device found: ") + name + " (" + port + ")");

    handle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (handle == INVALID_HANDLE_VALUE)
    {
        DWORD error_code = GetLastError();
        LOG("Error opening port: " + to_string(error_code));
        cerr << "Error opening port: " << error_code << endl;
        return;
    }

    LOG("Port opened successfully.");

    DCB dcb = {};
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(handle, &dcb))
    {
        LOG("Failed to get port state.");
        return;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(handle, &dcb))
    {
        LOG("Failed to set port state.");
        return;
    }

    COMMTIMEOUTS cto = {};
    cto.ReadIntervalTimeout = 50;
    cto.ReadTotalTimeoutConstant = 100;
    cto.ReadTotalTimeoutMultiplier = 10;
    cto.WriteTotalTimeoutConstant = 50;
    cto.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(handle, &cto))
    {
        LOG("Failed to set timeouts.");
        return;
    }

    LOG("Port initialized successfully.");
    cout << "Successfully connected!" << endl;
}

bool Arduino::GetDevice(LPCSTR name, LPSTR port)
{
    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);

    if (device_info == INVALID_HANDLE_VALUE)
    {
        LOG("Failed to get device information (SetupDiGetClassDevs returned INVALID_HANDLE_VALUE).");
        return false;
    }

    vector<DeviceInfo> devices;
    vector<regex> patterns =
    {
        regex("VID_2341&PID_.*"),
        regex("VID_2A03&PID_.*"),
        regex("VID_1A86&PID_.*")
    };

    // Load configuration
    DeviceInfo config_device;
    bool found_device = false;
    bool config_loaded = Arduino::LoadConfiguration("device_config.cfg", config_device);

    // Collect system devices
    LOG("Enumerating connected devices...");
    DWORD count = 0;
    SP_DEVINFO_DATA dev_info_data = {};
    dev_info_data.cbSize = sizeof(dev_info_data);

    while (SetupDiEnumDeviceInfo(device_info, count++, &dev_info_data))
    {
        DeviceInfo system_device;

        if (Arduino::ExtractProperties(device_info, dev_info_data, system_device))
        {
            devices.push_back(system_device);
            LOG("Detected device: FRIENDLYNAME = " + system_device.friendly_name + ", HARDWAREID = " + system_device.hardware_id + ", PORT = " + system_device.port);

            // Check against configuration file
            if (config_loaded && system_device.friendly_name == config_device.friendly_name && system_device.hardware_id == config_device.hardware_id)
            {
                strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                LOG("Using saved device: FRIENDLYNAME = " + system_device.friendly_name + ", HARDWAREID = " + system_device.hardware_id + ", PORT = " + system_device.port);
                found_device = true;
                break;
            }

            // Check against VID/PID patterns or name
            if (!found_device && name && system_device.friendly_name.find(name) != string::npos)
            {
                strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                LOG("Device matched by FRIENDLYNAME: " + system_device.friendly_name);
                found_device = true;
                break;
            }

            for (const auto& pattern : patterns)
            {
                if (regex_search(system_device.hardware_id, pattern))
                {
                    strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                    LOG("Device matched by VID/PID: " + system_device.hardware_id);
                    found_device = true;
                    break;
                }
            }
        }

        if (found_device)
            break;
    }

    SetupDiDestroyDeviceInfoList(device_info);

    // Handle case where no device was found
    if (!found_device)
    {
        if (config_loaded)
        {
            LOG("Saved device not found in the system. Deleting configuration file.");
            remove("device_config.cfg");
        }

        if (devices.empty())
        {
            LOG("No devices found in the system.");
            return false;
        }

        LOG("Prompting user to select a device...");
        return Arduino::SelectDevice(devices, port);
    }

    return true;
}

bool Arduino::LoadConfiguration(const string& file_name, DeviceInfo& config_device)
{
    string line;
    ifstream config(file_name);

    if (!config)
    {
        LOG("Configuration file not found.");
        return false;
    }

    while (getline(config, line))
    {
        if (line.find("FRIENDLYNAME=") == 0)
        {
            config_device.friendly_name = line.substr(13);
        }
        else if (line.find("HARDWAREID=") == 0)
        {
            config_device.hardware_id = line.substr(11);
        }
        else if (line.find("PORT=") == 0)
        {
            config_device.port = line.substr(5);
        }
    }

    if (!config_device.friendly_name.empty() && !config_device.hardware_id.empty())
    {
        LOG("Loaded device from configuration: FRIENDLYNAME = " + config_device.friendly_name + ", HARDWAREID = " + config_device.hardware_id + ", PORT = " + config_device.port);
        return true;
    }

    LOG("Configuration file is incomplete or corrupted.");
    return false;
}

bool Arduino::ExtractProperties(HDEVINFO device_info, SP_DEVINFO_DATA& dev_info_data, DeviceInfo& device)
{
    BYTE buffer[256];

    if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, sizeof(buffer), NULL))
    {
        device.friendly_name = string((LPCSTR)buffer);

        LPCSTR start = strchr((LPCSTR)buffer, '(');
        LPCSTR end = strchr((LPCSTR)buffer, ')');

        if (start && end && end > start)
        {
            device.port = string(start + 1, end - start - 1);
        }
    }

    if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_HARDWAREID, NULL, buffer, sizeof(buffer), NULL))
    {
        device.hardware_id = string((LPCSTR)buffer);
    }

    return !device.friendly_name.empty() && !device.hardware_id.empty();
}

bool Arduino::SelectDevice(const vector<DeviceInfo>& devices, LPSTR port)
{
    Utils utils;
    utils.PrintCenteredText("The following devices were detected:");

    for (size_t i = 0; i < devices.size(); ++i)
    {
        string device_info = "[" + to_string(i + 1) + "] FRIENDLYNAME: " + devices[i].friendly_name + ", HARDWAREID: " + devices[i].hardware_id + ", PORT: " + devices[i].port;
        utils.PrintCenteredText(device_info);
    }

    utils.PrintCenteredText("Please select the device you want to use by entering its number (0 to cancel): ", false);

    int choice;
    cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(devices.size()))
    {
        const auto& selected_device = devices[choice - 1];
        strncpy_s(port, 100, selected_device.port.c_str(), selected_device.port.size());
        LOG("User selected device: " + selected_device.friendly_name);

        ofstream config("device_config.cfg");

        if (config)
        {
            config << "FRIENDLYNAME=" << selected_device.friendly_name << endl;
            config << "HARDWAREID=" << selected_device.hardware_id << endl;
            config << "PORT=" << selected_device.port << endl;

            LOG("Device selection saved to configuration.");
        }

        return true;
    }

    utils.PrintCenteredText("No device selected by the user.");
    LOG("No device selected by the user.");

    return false;
}

bool Arduino::WriteMessage(const string& message) const
{
    DWORD bytes = 0;
    BOOL result = WriteFile(handle, message.c_str(), message.size() + 1, &bytes, nullptr);

    if (result == 0 || bytes != message.size() + 1)
    {
        LOG("Failed to send message: " + message);
        cerr << "Failed to send message!" << endl;
        return false;
    }

    LOG("Message sent: " + message);
    return true;
}

void Arduino::LogMessage(const string& message)
{
    static ofstream log_file("arduino_debug.log", ios::app);

    if (!log_file.is_open())
    {
        cerr << "Error opening the log file." << endl;
        return;
    }

    tm local;
    time_t now = time(nullptr);
    localtime_s(&local, &now);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local);

    log_file << "[" << buffer << "] " << message << std::endl;
}