#include "Arduino.h"
#include "Logger.h"

Arduino::~Arduino()
{
    if (serial_port.is_open())
    {
        serial_port.close();
        Logger::LogMessage("Disconnected from Arduino.", boost::log::trivial::debug);
    }
}

Arduino::Arduino(LPCSTR name) : io_context(), serial_port(io_context)
{
    char port[100] = "\\.\\";

    Logger::LogMessage("Searching for device...");
    while (!GetDevice(name, port))
    {
        Logger::LogMessage("Device not found. Retrying...");
        sleep_for(milliseconds(1000));
    }

    Logger::LogMessage(string("Device found: ") + name + " (" + port + ")");

    try
    {
        serial_port.open(port);

        if (!serial_port.is_open())
        {
            Logger::LogMessage("Error opening port: Serial port not open after attempt.");
            cerr << "Error opening port: Serial port not open after attempt." << endl;
            return;
        }
    }
    catch (boost::system::system_error& error)
    {
        DWORD error_code = error.code().value();
        Logger::LogMessage("Error opening port: " + to_string(error_code) + " - " + error.what());
        cerr << "Error opening port: " << error_code << " - " << error.what() << endl;
        return;
    }

    Logger::LogMessage("Port opened successfully.");

    try
    {
        serial_port.set_option(serial_port::baud_rate(9600));
        serial_port.set_option(serial_port::baud_rate(9600));
        serial_port.set_option(serial_port::character_size(8));
        serial_port.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        serial_port.set_option(serial_port::parity(serial_port::parity::none));
        serial_port.set_option(serial_port::flow_control(serial_port::flow_control::none));
    }
    catch (boost::system::system_error& error)
    {
        Logger::LogMessage("Failed to set port options: " + string(error.what()));
        cerr << "Failed to set port options: " << error.what() << endl;
        serial_port.close();
        return;
    }

    Logger::LogMessage("Port initialized successfully.");
    cout << "Successfully connected!" << endl;
}

bool Arduino::GetDevice(LPCSTR name, LPSTR port)
{
    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);

    if (device_info == INVALID_HANDLE_VALUE)
    {
        Logger::LogMessage("Failed to get device information (SetupDiGetClassDevs returned INVALID_HANDLE_VALUE).");
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
    Logger::LogMessage("Enumerating connected devices...");
    DWORD count = 0;
    SP_DEVINFO_DATA dev_info_data = {};
    dev_info_data.cbSize = sizeof(dev_info_data);

    while (SetupDiEnumDeviceInfo(device_info, count++, &dev_info_data))
    {
        DeviceInfo system_device;

        if (Arduino::ExtractProperties(device_info, dev_info_data, system_device))
        {
            devices.push_back(system_device);
            Logger::LogMessage("Detected device: FRIENDLYNAME = " + system_device.friendly_name + ", HARDWAREID = " + system_device.hardware_id + ", PORT = " + system_device.port);

            // Check against configuration file
            if (config_loaded && system_device.friendly_name == config_device.friendly_name && system_device.hardware_id == config_device.hardware_id)
            {
                strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                Logger::LogMessage("Using saved device: FRIENDLYNAME = " + system_device.friendly_name + ", HARDWAREID = " + system_device.hardware_id + ", PORT = " + system_device.port);
                found_device = true;
                break;
            }

            // Check against VID/PID patterns or name
            if (!found_device && name && system_device.friendly_name.find(name) != string::npos)
            {
                strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                Logger::LogMessage("Device matched by FRIENDLYNAME: " + system_device.friendly_name);
                found_device = true;
                break;
            }

            for (const auto& pattern : patterns)
            {
                if (regex_search(system_device.hardware_id, pattern))
                {
                    strncpy_s(port, 100, system_device.port.c_str(), system_device.port.size());
                    Logger::LogMessage("Device matched by VID/PID: " + system_device.hardware_id);
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
            Logger::LogMessage("Saved device not found in the system. Deleting configuration file.");
            remove("device_config.cfg");
        }

        if (devices.empty())
        {
            Logger::LogMessage("No devices found in the system.");
            return false;
        }

        Logger::LogMessage("Prompting user to select a device...");
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
        Logger::LogMessage("Configuration file not found.", boost::log::trivial::debug);
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
        Logger::LogMessage("Loaded device from configuration: FRIENDLYNAME = " + config_device.friendly_name + ", HARDWAREID = " + config_device.hardware_id + ", PORT = " + config_device.port);
        return true;
    }

    Logger::LogMessage("Configuration file is incomplete or corrupted.");
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
        Logger::LogMessage("User selected device: " + selected_device.friendly_name);

        ofstream config("device_config.cfg");

        if (config)
        {
            config << "FRIENDLYNAME=" << selected_device.friendly_name << endl;
            config << "HARDWAREID=" << selected_device.hardware_id << endl;
            config << "PORT=" << selected_device.port << endl;

            Logger::LogMessage("Device selection saved to configuration.");
        }

        return true;
    }

    utils.PrintCenteredText("No device selected by the user.");
    Logger::LogMessage("No device selected by the user.");

    return false;
}

bool Arduino::WriteMessage(const string& message)
{
    if (!serial_port.is_open())
    {
        Logger::LogMessage("Attempt to write to a closed serial port");
        cerr << "Attempt to write to a closed serial port" << endl;
        return false;
    }

    try
    {
        write(serial_port, buffer(message.c_str(), message.size() + 1));
        Logger::LogMessage("Message sent: " + message);
        return true;
    }
    catch (boost::system::system_error& error)
    {
        Logger::LogMessage("Failed to send message: " + message + " - " + error.what());
        cerr << "Failed to send message: " << message << " - " << error.what() << endl;
        return false;
    }
}