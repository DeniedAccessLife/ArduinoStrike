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

	string line;
	istringstream iss(ascii);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SHORT startY = static_cast<SHORT>(height - count(ascii.begin(), ascii.end(), '\n') - 1) / 2;

	while (getline(iss, line))
	{
		SHORT padding = static_cast<SHORT>(width - line.size()) / 2;

		COORD coord = { padding, startY++ };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		cout << line;
	}

	cout << endl << endl;
}

void Utils::PrintHotkeys(const string& keys)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &csbi);

	size_t start = 0, end;
	vector<string> hotkeys;
	const int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	while ((end = keys.find(" | ", start)) != string::npos)
	{
		hotkeys.emplace_back(keys.substr(start, end - start));
		start = end + 3;
	}

	hotkeys.emplace_back(keys.substr(start));

	if (hotkeys.empty()) return;

	string top, bottom;
	const size_t split_at = hotkeys.size() / 2;

	for (size_t i = 0; i < split_at; ++i)
	{
		if (i > 0) top += " | ";
		top += hotkeys[i];
	}

	for (size_t i = split_at; i < hotkeys.size(); ++i)
	{
		if (i > split_at) bottom += " | ";
		bottom += hotkeys[i];
	}

	const int top_pad = max(0, (width - static_cast<int>(top.length())) / 2);
	const int bottom_pad = max(0, (width - static_cast<int>(bottom.length())) / 2);

	SetConsoleCursorPosition(console, { static_cast<SHORT>(top_pad), static_cast<SHORT>(csbi.srWindow.Top) });
	cout << top;

	SetConsoleCursorPosition(console, { static_cast<SHORT>(bottom_pad), static_cast<SHORT>(csbi.srWindow.Bottom) });
	cout << bottom;
}

void Utils::PrintCenteredText(const string& text, bool wrap)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &csbi);

	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	SHORT padding = static_cast<SHORT>((width - text.size()) / 2);

	COORD coord = { padding, csbi.dwCursorPosition.Y };
	SetConsoleCursorPosition(console, coord);
	cout << text;

	if (wrap)
	{
		cout << endl;
	}
	else
	{
		SetConsoleCursorPosition(console, { static_cast<SHORT>(padding + text.size()), coord.Y } );
	}
}

void Utils::ConsoleClear()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD written;
	CONSOLE_CURSOR_INFO cursor;
	CONSOLE_SCREEN_BUFFER_INFO screen;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(console, topLeft);

	GetConsoleCursorInfo(console, &cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(console, &cursor);
}

void Utils::SetConsoleMode(const string& title)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(console, &buffer);

	COORD coord = {};
	coord.X = buffer.srWindow.Right - buffer.srWindow.Left + 1;
	coord.Y = buffer.srWindow.Bottom - buffer.srWindow.Top + 1;

	SetConsoleScreenBufferSize(console, coord);
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