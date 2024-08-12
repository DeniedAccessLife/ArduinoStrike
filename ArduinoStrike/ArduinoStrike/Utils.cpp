#include "Utils.h"

void Utils::Install()
{
	string random = GenerateRandomData(16);

	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	path executable = buffer;
	path tempFolder = temp_directory_path();

	if (executable.string().substr(0, tempFolder.string().size()) != tempFolder.string())
	{
		path destinationFolder = tempFolder / random;
		path destinationApplication = destinationFolder / (random + ".exe");

		try
		{
			create_directories(destinationFolder);
			copy_file(executable, destinationApplication, copy_options::overwrite_existing);

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			if (CreateProcessA(NULL, (LPSTR)destinationApplication.string().c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				ExitProcess(0);
			}
			else
			{
				cerr << "Failed to create process." << endl;
				exit(1);
			}
		}
		catch (exception& ex)
		{
			cerr << "Error: " << ex.what() << endl;
			exit(1);
		}
	}

	SetConsoleMode(random);
}

void Utils::LoadConfig(Config& config)
{
	ifstream file("Settings.cfg");

	if (!file.is_open())
	{
		config.bhop = GetValidatedIntInput("Enter bhop boolean value (1/0) -> ", 0, 1);
		config.rapidFire = GetValidatedIntInput("Enter rapid-fire boolean value (1/0) -> ", 0, 1);
		config.sensitivity = GetValidatedIntInput("Enter sensitivity integer value (1-8) -> ", 1, 8);
		config.confirmationKey = GetValidatedKeyInput("Enter recoil control confirmation key (0/VK_CODE) -> ");

		ofstream out("Settings.cfg");
		out << config.bhop << endl << config.rapidFire << endl << config.sensitivity << endl << hex << config.confirmationKey;
		out.close();

		cout << "Configuration successfully saved!" << endl;
	}
	else
	{
		file >> config.bhop;
		file >> config.rapidFire;
		file >> config.sensitivity;
		file >> hex >> config.confirmationKey;
		file.close();

		if (!ValidateConfig(config))
		{
			cerr << "Invalid values in config file. Please enter valid values!" << endl;
			remove("Settings.cfg");
			exit(1);
		}

		cout << "Configuration successfully loaded!" << endl;
	}
}

int Utils::GetValidatedIntInput(const string& prompt, int min, int max)
{
	string input;
	int value = -1;

	do
	{
		cout << prompt;
		getline(cin, input);
		istringstream iss(input);

		if (!(iss >> value) || value < min || value > max)
		{
			value = -1;

			if (min == 0 && max == 1)
			{
				cout << "Invalid input! Please enter 0 or 1." << endl << endl;
			}
			else
			{
				cout << "Invalid input! Please enter an integer between " << min << " and " << max << "." << endl << endl;
			}
		}
	} while (value == -1);

	return value;
}

int Utils::GetValidatedKeyInput(const string& prompt)
{
	string input;
	int value = -1;

	do
	{
		cout << prompt;
		getline(cin, input);
		istringstream iss(input);

		if (!(iss >> hex >> value) || (value != 0 && (value < 0x01 || value > 0xFE)))
		{
			value = -1;
			cout << "Invalid input! Please enter 0 or VK_CODE." << endl << endl;
		}
	} while (value == -1);

	return value;
}

bool Utils::ValidateConfig(Config& config)
{
	bool validBhop = (config.bhop == 0 || config.bhop == 1);
	bool validRapidFire = (config.rapidFire == 0 || config.rapidFire == 1);
	bool validSensitivity = (config.sensitivity >= 1 && config.sensitivity <= 8);
	bool validConfirmationKey = (config.confirmationKey == 0 || (config.confirmationKey >= 0x01 && config.confirmationKey <= 0xFE));

	return validBhop && validRapidFire && validSensitivity && validConfirmationKey;
}

void Utils::PrintAscii(const string& asciiArt)
{
	ConsoleClear();

	vector<string> lines;
	istringstream iss(asciiArt);

	for (string line; getline(iss, line);)
	{
		lines.push_back(line);
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SHORT startY = static_cast<SHORT>(height - lines.size()) / 2;

	for (SHORT i = 0; i < static_cast<SHORT>(lines.size()); i++)
	{
		const string &line = lines[i];

		SHORT padding = static_cast<SHORT>(width - line.size()) / 2;

		COORD coord = {};
		coord.X = padding;
		coord.Y = startY + i;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		cout << line;
	}
}

void Utils::PrintHotkeys(const string& hotkeys)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int padding = (width - hotkeys.size()) / 2;

	COORD coord = {};
	coord.X = 0;
	coord.Y = csbi.srWindow.Bottom;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	for (int i = 0; i < padding; i++)
	{
		cout << " ";
	}

	cout << hotkeys;
}

void Utils::ConsoleClear()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD written;
	CONSOLE_CURSOR_INFO cursorInfo;
	CONSOLE_SCREEN_BUFFER_INFO screen;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(console, topLeft);

	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}

void Utils::SetConsoleMode(const string& title)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(console, &buffer);

	COORD size = {};
	size.X = buffer.srWindow.Right - buffer.srWindow.Left + 1;
	size.Y = buffer.srWindow.Bottom - buffer.srWindow.Top + 1;

	SetConsoleScreenBufferSize(console, size);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SetConsoleTitleA(title.c_str());
}

string Utils::GenerateRandomData(int length)
{
	string result;
	srand((unsigned int)time(NULL));

	for (int i = 0; i < length; i++)
	{
		int r = rand() % 36;

		if (r < 10)
		{
			result += to_string(r);
		}
		else
		{
			result += char(r - 10 + 'a');
		}
	}

	return result;
}