#include "Utils.h"

Utils::Utils()
{
	Install();
}

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

void Utils::PrintAscii(const string& ascii)
{
	ConsoleClear();

	vector<string> lines;
	istringstream iss(ascii);

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

void Utils::PrintHotkeys(const string& keys)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int padding = (width - keys.size()) / 2;

	COORD coord = {};
	coord.X = 0;
	coord.Y = csbi.srWindow.Bottom;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	for (int i = 0; i < padding; i++)
	{
		cout << " ";
	}

	cout << keys;
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