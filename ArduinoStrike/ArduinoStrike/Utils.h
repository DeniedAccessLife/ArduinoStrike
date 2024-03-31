#pragma once
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <sstream>
#include <fstream>
#include <future>

using namespace std;
namespace fs = experimental::filesystem;
using namespace fs;
using namespace chrono;
using namespace this_thread;

enum Weapon
{
	OFF,
	UMP,
	M4A1,
	M4A4,
	AK47,
	GALIL,
	FAMAS
};

struct Config
{
	int bhop;
	int rapid_fire;
	int sensitivity;
};

class Utils
{
public:
	static void Install();
	static bool validateConfig(Config & config);
	static void LoadConfig(Config & config);
	static void ConsoleClear();
	static void PrintHotkeys(const string &hotkeys);
	static void PrintAscii(const string &asciiArt);
	static Weapon weaponState(Weapon weapon);

private:
	static string randomData(int length);
	static void SetConsoleMode(const string &title);
};