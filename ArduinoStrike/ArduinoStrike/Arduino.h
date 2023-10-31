#pragma once
#pragma comment(lib, "Setupapi.lib")
#include <Windows.h>
#include <iostream>
#include <devguid.h>
#include <SetupAPI.h>

using namespace std;

class Arduino
{
public:
	Arduino(LPCSTR device_name);
	~Arduino();
	bool send_data(const string& message) const;
	bool available() const;
	string readStringUntil(char delimiter) const;
	static bool get_device(LPCSTR friendly_name, LPSTR com_port);

private:
	HANDLE m_hArduino;
};