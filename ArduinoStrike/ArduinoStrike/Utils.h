#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

struct Config
{
	int bhop;
	int rapidFire;
	int sensitivity;
};

class Utils
{
public:
	static void Install();
	static void LoadConfig(Config& config);
	static bool ValidateConfig(Config& config);

	static void PrintAscii(const string& asciiArt);
	static void PrintHotkeys(const string& hotkeys);

private:
	static void ConsoleClear();
	static string GenerateRandomData(int length);
	static void SetConsoleMode(const string& title);
};