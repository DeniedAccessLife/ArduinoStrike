#pragma once
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <sstream>

using namespace std;
namespace fs = experimental::filesystem;
using namespace fs;
using namespace chrono;
using namespace this_thread;

enum Weapon { OFF, UMP, M4A1, M4A4, AK47, GALIL, FAMAS };

class Utils
{
public:
	static void Install();
	static void ConsoleClear();
	static void PrintHotkeys(const string &hotkeys);
	static void PrintAscii(const string &asciiArt);

private:
	static string randomData(int length);
	static void SetConsoleMode(const string &title);
};