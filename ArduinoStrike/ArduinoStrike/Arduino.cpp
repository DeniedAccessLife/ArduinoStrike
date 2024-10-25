#include "Arduino.h"

Arduino::~Arduino()
{
    if (m_arduinoHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_arduinoHandle);
    }
}

Arduino::Arduino(LPCSTR device_name) : m_arduinoHandle(INVALID_HANDLE_VALUE)
{
    char port[100] = "\\.\\";

    while (!this->GetDevice(device_name, port))
    {
        sleep_for(milliseconds(1000));
    }

    m_arduinoHandle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (m_arduinoHandle == INVALID_HANDLE_VALUE)
    {
        cerr << "Error opening port: " << GetLastError() << endl;
        return;
    }

    DCB dcb = {};
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(m_arduinoHandle, &dcb))
        return;

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(m_arduinoHandle, &dcb))
        return;

    COMMTIMEOUTS cto = {};
    cto.ReadIntervalTimeout = 50;
    cto.ReadTotalTimeoutConstant = 50;
    cto.ReadTotalTimeoutMultiplier = 10;
    cto.WriteTotalTimeoutConstant = 50;
    cto.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(m_arduinoHandle, &cto))
        return;

    cout << "Successfully connected!" << endl;
}

bool Arduino::IsAvailable() const
{
    DWORD errors;
    COMSTAT status;

    if (!ClearCommError(m_arduinoHandle, &errors, &status))
    {
        cerr << "Error checking available data: " << GetLastError() << endl;
        return false;
    }

    return status.cbInQue > 0;
}

bool Arduino::GetDevice(LPCSTR friendly_name, LPSTR com_port)
{
    const char com[] = "COM";
    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);

    if (device_info == INVALID_HANDLE_VALUE)
        return false;

    SP_DEVINFO_DATA dev_info_data = {};
    dev_info_data.cbSize = sizeof(dev_info_data);

    DWORD device_count = 0;

    while (SetupDiEnumDeviceInfo(device_info, device_count++, &dev_info_data))
    {
        BYTE buff[256] = { 0 };

        if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, nullptr, buff, sizeof(buff), nullptr))
        {
            LPCSTR port_name_pos = strstr(reinterpret_cast<LPCSTR>(buff), com);

            if (port_name_pos == nullptr)
                continue;

            if (strstr(reinterpret_cast<LPCSTR>(buff), friendly_name))
            {
                strncpy_s(com_port, 100, port_name_pos, strlen(port_name_pos) - 1);
                com_port[strlen(port_name_pos) - 1] = '\0';
                SetupDiDestroyDeviceInfoList(device_info);

                return true;
            }
        }
    }

    SetupDiDestroyDeviceInfoList(device_info);
    return false;
}

bool Arduino::WriteMessage(const string& message) const
{
    DWORD bw = 0;
    BOOL result = WriteFile(m_arduinoHandle, message.c_str(), message.size() + 1, &bw, nullptr);

    if (result == 0 || bw != message.size() + 1)
    {
        cerr << "Failed to send message!" << endl;
        return false;
    }

    return true;
}

string Arduino::ReceiveMessage(char delimiter) const
{
    string result;
    char buffer[256];
    DWORD bytesRead = 0;

    while (this->IsAvailable())
    {
        memset(buffer, 0, sizeof(buffer));

        if (!ReadFile(m_arduinoHandle, buffer, sizeof(buffer), &bytesRead, nullptr))
            break;

        for (DWORD i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == delimiter)
            {
                return result;
            }

            result += buffer[i];
        }
    }

    return result;
}