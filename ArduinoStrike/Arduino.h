#pragma once
#pragma comment(lib, "Setupapi.lib")

#include <thread>
#include <iostream>
#include <windows.h>
#include <devguid.h>
#include <SetupAPI.h>

using namespace std;
using namespace chrono;
using namespace this_thread;

class Arduino
{
public:
    ~Arduino();
    Arduino(LPCSTR device_name);

    bool WriteMessage(const string& message) const;
    string ReceiveMessage(char delimiter) const;

private:
    HANDLE m_arduinoHandle;
    bool IsAvailable() const;
    bool GetDevice(LPCSTR friendly_name, LPSTR com_port);
};