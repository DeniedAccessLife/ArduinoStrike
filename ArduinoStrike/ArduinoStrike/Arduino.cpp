#include "Arduino.h"

Arduino::~Arduino()
{
	CloseHandle(m_hArduino);
}

Arduino::Arduino(LPCSTR device_name)
{
	char port[100] = "\\.\\";

	while (!Arduino::get_device(device_name, port))
	{
		Sleep(1000);
	}

	m_hArduino = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (m_hArduino == INVALID_HANDLE_VALUE)
	{
		printf("Error opening port: %d\n", GetLastError());
		return;
	}

	DCB dcb = { };
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(m_hArduino, &dcb))
		return;

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;

	if (!SetCommState(m_hArduino, &dcb))
		return;

	COMMTIMEOUTS cto = { };
	cto.ReadIntervalTimeout = 50;
	cto.ReadTotalTimeoutConstant = 50;
	cto.ReadTotalTimeoutMultiplier = 10;
	cto.WriteTotalTimeoutConstant = 50;
	cto.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(m_hArduino, &cto))
		return;

	printf("Successfully connected!\n");
}

bool Arduino::get_device(LPCSTR friendly_name, LPSTR com_port)
{
	const char com[] = "COM";

	HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
	if (device_info == INVALID_HANDLE_VALUE)
		return false;

	SP_DEVINFO_DATA dev_info_data = { };
	dev_info_data.cbSize = sizeof(dev_info_data);

	DWORD device_count = 0;
	while (SetupDiEnumDeviceInfo(device_info, device_count++, &dev_info_data))
	{
		BYTE buff[256] = { 0 };
		if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, nullptr, buff, sizeof(buff), nullptr))
		{
			DWORD out_buff_len = strlen(com_port);
			LPCSTR port_name_pos = strstr(reinterpret_cast<LPCSTR>(buff), com);
			if (port_name_pos == nullptr)
				continue;

			DWORD len = out_buff_len + strlen(port_name_pos);
			if (strstr(reinterpret_cast<LPCSTR>(buff), friendly_name))
			{
				for (DWORD i = 0; i < len; i++, out_buff_len++)
					com_port[out_buff_len] = port_name_pos[i];

				com_port[strlen(com_port) - 1] = 0;
				return true;
			}
		}
	}

	return false;
}

bool Arduino::send_data(const string &message) const
{
	DWORD bw = 0;
	BOOL result = WriteFile(m_hArduino, message.c_str(), message.size() + 1, &bw, nullptr);

	if (result == 0 || bw != message.size() + 1)
	{
		printf("Failed to send message!\n");
		exit(1);
	}

	return true;
}

bool Arduino::available() const
{
	DWORD errors;
	COMSTAT status;

	if (!ClearCommError(m_hArduino, &errors, &status))
	{
		printf("Error checking available data: %d\n", GetLastError());
		return false;
	}

	return status.cbInQue > 0;
}

string Arduino::readStringUntil(char delimiter) const
{
	string result;
	char buffer[256];
	DWORD bytesRead = 0;

	while (this->available())
	{
		memset(buffer, 0, sizeof(buffer));

		if (!ReadFile(m_hArduino, buffer, sizeof(buffer), &bytesRead, nullptr))
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