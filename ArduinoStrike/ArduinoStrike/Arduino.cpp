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

    while (!GetDevice(device_name, port))
    {
        sleep_for(milliseconds(1000));
    }

    this->m_arduinoHandle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (this->m_arduinoHandle)
    {
        DCB dcb = { 0 };
        dcb.DCBlength = sizeof(dcb);
        if (!GetCommState(this->m_arduinoHandle, &dcb))
        {
            printf("GetCommState() failed\n");
            CloseHandle(this->m_arduinoHandle);
        }

        dcb.BaudRate = CBR_9600;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;
        if (!SetCommState(this->m_arduinoHandle, &dcb))
        {
            printf("SetCommState() failed\n");
            CloseHandle(this->m_arduinoHandle);
        }

        COMMTIMEOUTS cto = { 0 };
        cto.ReadIntervalTimeout = 50;
        cto.ReadTotalTimeoutConstant = 50;
        cto.ReadTotalTimeoutMultiplier = 10;
        cto.WriteTotalTimeoutConstant = 50;
        cto.WriteTotalTimeoutMultiplier = 10;
        if (!SetCommTimeouts(this->m_arduinoHandle, &cto))
        {
            printf("SetCommTimeouts() failed\n");
            CloseHandle(this->m_arduinoHandle);
        }
        cout << "Successfully connected!" << endl;
    }
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
    char com[] = "COM";
    bool status = false;

    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (device_info == INVALID_HANDLE_VALUE) return false;

    SP_DEVINFO_DATA dev_info_data;
    dev_info_data.cbSize = sizeof(dev_info_data);

    DWORD count = 0;

    while (SetupDiEnumDeviceInfo(device_info, count++, &dev_info_data))
    {
        BYTE buffer[256];
        if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, sizeof(buffer), NULL))
        {
            DWORD i = strlen(com_port);
            LPCSTR lp_pos = strstr((LPCSTR)buffer, com);
            DWORD len = i + (lp_pos ? strlen(lp_pos) : 0);

            if (strstr((LPCSTR)buffer, friendly_name) && lp_pos)
            {
                for (DWORD j = 0; i < len; i++, j++)
                {
                    com_port[i] = lp_pos[j];
                }

                com_port[i - 1] = '\0';
                status = true;
                break;
            }
        }
    }

    SetupDiDestroyDeviceInfoList(device_info);
    return status;
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