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

class Arduino
{
public:
    ~Arduino();
    Arduino(LPCSTR name);
    bool WriteMessage(const string& message) const;

private:
    HANDLE handle;
    bool IsAvailable() const;
    bool GetDevice(LPCSTR name, LPSTR port);
    static void LogMessage(const string& message);
};