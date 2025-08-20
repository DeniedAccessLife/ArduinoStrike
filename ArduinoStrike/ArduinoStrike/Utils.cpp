#include "Utils.h"

Utils::Utils()
{
	Install();
}

void Utils::Install()
{
    // Get the full path to the current executable
    char self[MAX_PATH];
    GetModuleFileNameA(nullptr, self, MAX_PATH);
    Logger::LogMessage("Install: Current executable path: " + string(self));

    // Get the system's temporary directory path
    const auto temp = temp_directory_path();
    Logger::LogMessage("Install: Temp directory: " + temp.string());

    // Generate a random 16-character string for unique folder naming
    const string random = GenerateRandomData(16);
    Logger::LogMessage("Install: Generated random data: " + random);

    // Check if we're already running from a temporary directory
    // If not, we need to copy ourselves there and restart
    if (!path(self).string().starts_with(temp.string()))
    {
        Logger::LogMessage("Install: Not in temp directory, starting self-copy process");
        
        // Create target directory path: temp/random_string/
        const auto target_directory = temp / random;

        // Create target executable path: temp/random_string/random_string.exe
        const auto target_executable = target_directory / (random + ".exe");
        Logger::LogMessage("Install: Target path: " + target_executable.string());

        try
        {
            // Create the temporary directory structure
            create_directories(target_directory);
            Logger::LogMessage("Install: Created temp directory successfully");

            // Copy the current executable to the temporary location
            copy_file(self, target_executable, copy_options::overwrite_existing);
            Logger::LogMessage("Install: Executable copied successfully");

            // Prepare process creation structures
            STARTUPINFOA si { sizeof(si) };
            PROCESS_INFORMATION pi;

            // Extract and prepare command line arguments
            string arguments;

			// GetCommandLineA returns pointer to full command line string
            if (LPSTR raw_command_line = GetCommandLineA())
            {
                // Convert raw command line pointer to string_view for easier manipulation
                string_view full_command_line(raw_command_line);
                
                // Find the first space to separate executable name from arguments
                if (auto space_position = full_command_line.find(' '); space_position != string_view::npos)
                {
                    // Extract everything after the executable name (the actual arguments)
                    arguments = full_command_line.substr(space_position + 1);
                    
                    // Only log if we actually have arguments
                    if (!arguments.empty())
                    {
                        Logger::LogMessage("Install: Extracted arguments: " + arguments);
                    }
					else
					{
						Logger::LogMessage("Install: No command line arguments found");
					}
                }
            }
            else
            {
                Logger::LogMessage("Install: Failed to get command line");
            }

            // Launch the copied executable with the same arguments
			if (CreateProcessA(target_executable.string().c_str(), arguments.empty() ? nullptr : const_cast<char*>(arguments.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
            {
                Logger::LogMessage("Install: New process launched successfully, exiting current process");
                
                // Clean up process handles to prevent resource leaks
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);

                // Exit the current process - the new one will continue running
                ExitProcess(0);
            }

            // If process creation failed, throw an error
            Logger::LogMessage("Install: CreateProcess failed", boost::log::trivial::error);
            throw runtime_error("CreateProcess failed!");
        }
        catch (const exception& ex)
        {
            // Log the error and exit if installation fails
            Logger::LogMessage("Install: Installation failed: " + string(ex.what()), boost::log::trivial::error);
            cerr << "Installation failed: " << ex.what() << endl;
            ExitProcess(1);
        }
    }
    else
    {
        Logger::LogMessage("Install: Already running from temp directory, skipping self-copy");
    }

    // Set the console title to the random string
    SetConsoleMode(random);
    Logger::LogMessage("Install: Console mode set, installation complete");
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