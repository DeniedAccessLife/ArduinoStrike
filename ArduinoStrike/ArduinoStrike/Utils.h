#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>
#include <iomanip>

using namespace std;
using namespace filesystem;

struct Config
{
	int bhop;
	int rapidFire;
	int sensitivity;
	int confirmationKey;
};

class Utils
{
public:
	static void Install();
	static void LoadConfig(Config& config);

	static void PrintAscii(const string& asciiArt);
	static void PrintHotkeys(const string& hotkeys);

private:
	static void ConsoleClear();
	static bool ValidateConfig(Config& config);
	static string GenerateRandomData(int length);
	static void SetConsoleMode(const string& title);
	static int GetValidatedIntInput(const string& prompt, int min, int max);
	static int GetValidatedKeyInput(const string& prompt);
};