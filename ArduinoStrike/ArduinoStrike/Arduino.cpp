#include "Arduino.h"

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

bool Arduino::IsAvailable() const
{
    DWORD errors;
    COMSTAT status;

    if (!ClearCommError(handle, &errors, &status))
    {
        LOG("Error checking available data: " + to_string(GetLastError()));
        return false;
    }

    return status.cbInQue > 0;
}

bool Arduino::GetDevice(LPCSTR name, LPSTR port)
{
    bool status = false;
    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);

    if (device_info == INVALID_HANDLE_VALUE)
    {
        LOG("Failed to get device information.");
        return false;
    }

    DWORD count = 0;
    SP_DEVINFO_DATA dev_info_data = {};
    dev_info_data.cbSize = sizeof(dev_info_data);
    LOG("Searching for device: " + string(name));

    struct DeviceInfo
    {
        string port;
        string hardware_id;
        string friendly_name;
    };

    vector<DeviceInfo> devices;
    vector<regex> patterns = { regex("VID_2341&PID_.*"), regex("VID_1A86&PID_.*"), regex("VID_10C4&PID_.*") };

    // Step 1: Collecting all devices
    while (SetupDiEnumDeviceInfo(device_info, count++, &dev_info_data))
    {
        DeviceInfo info;
        BYTE buffer[256];

        // Reading FRIENDLYNAME
        if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, sizeof(buffer), NULL))
        {
            info.friendly_name = string((LPCSTR)buffer);
            LOG("Device detected with FRIENDLYNAME: " + info.friendly_name);

            // Reading PORT
            LPCSTR start = strchr((LPCSTR)buffer, '(');
            LPCSTR end = strchr((LPCSTR)buffer, ')');

            if (start && end && end > start)
            {
                info.port = string(start + 1, end - start - 1);
            }
        }

        // Reading HARDWAREID
        if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_HARDWAREID, NULL, buffer, sizeof(buffer), NULL))
        {
            info.hardware_id = string((LPCSTR)buffer);
            LOG("Device detected with HARDWAREID: " + info.hardware_id);
        }

        devices.push_back(info);
    }

    SetupDiDestroyDeviceInfoList(device_info);

    // Step 2: Checking and selecting the device
    for (const auto& device : devices)
    {
        LOG("Evaluating device: FRIENDLYNAME = " + device.friendly_name + ", HARDWAREID = " + device.hardware_id);

        // If the name is specified, check it
        if (name && !device.friendly_name.empty() && device.friendly_name.find(name) != string::npos)
        {
            strncpy_s(port, 100, device.port.c_str(), device.port.size());
            LOG("Device matched by FRIENDLYNAME: " + device.friendly_name);
            status = true;
            break;
        }

        // If the name does not match, check the VID/PID
        for (const auto& pattern : patterns)
        {
            if (!device.hardware_id.empty() && regex_search(device.hardware_id, pattern))
            {
                strncpy_s(port, 100, device.port.c_str(), device.port.size());
                LOG("Device matched by VID/PID: " + device.hardware_id);
                status = true;
                break;
            }
        }

        if (status) break;
    }

    if (!status)
    {
        LOG("No matching device found.");
    }

    return status;
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