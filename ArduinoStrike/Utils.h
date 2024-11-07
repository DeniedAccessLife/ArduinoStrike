#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

class Utils
{
public:
    Utils();
    void PrintAscii(const string& ascii);
    void PrintHotkeys(const string& keys);

private:
    void Install();
    void ConsoleClear();
    string GenerateRandomData(int length);
    void SetConsoleMode(const string& title);
};