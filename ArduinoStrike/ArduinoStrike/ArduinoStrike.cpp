#include "Utils.h"
#include "Arduino.h"
int sensitivity;

Weapon weapon = OFF;
Weapon weaponState(Weapon weapon)
{
	if (GetAsyncKeyState(VK_F12) & 1) return OFF;

	if (GetAsyncKeyState(VK_F1) & 1) return UMP;
	if (GetAsyncKeyState(VK_F2) & 1) return M4A1;
	if (GetAsyncKeyState(VK_F3) & 1) return M4A4;
	if (GetAsyncKeyState(VK_F4) & 1) return AK47;
	if (GetAsyncKeyState(VK_F5) & 1) return GALIL;
	if (GetAsyncKeyState(VK_F6) & 1) return FAMAS;

	return weapon;
}

int main()
{
	Utils::Install();
	Arduino arduino("Arduino Leonardo");

	char input[10];
	do
	{
		printf("Enter sensitivity -> ");
		fgets(input, sizeof(input), stdin);
	}
	while (sscanf_s(input, "%d", &sensitivity) != 1 || sensitivity < 1 || sensitivity > 8);

	string text =
	R"(__      __       _   _            )" "\n"
	R"(\ \    / /__ _ _| |_(_)_ _  __ _  )" "\n"
	R"( \ \/\/ / _ \ '_| / / | ' \/ _` | )" "\n"
	R"(  \_/\_/\___/_| |_\_\_|_||_\__, | )" "\n"
	R"(                           |___/  )" "\n"
	R"(                                  )" "\n"
	R"(   CTRL + C to stop the program   )" "\n";

	Utils::PrintAscii(text);
	Utils::PrintHotkeys("[F1] - OFF | [F1] - UMP | [F2] - M4A1 | [F3] - M4A4 | [F4] - AK47 | [F5] - GALIL | [F6] - FAMAS");

	while (true)
	{
		weapon = weaponState(weapon);
		string message = arduino.readStringUntil('\n');

		if (message.rfind("ARDUINO_INITIATED", 0) != 0) 
		{
			double modifier = 2.52 / sensitivity;

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				vector<double> x, y;
				vector<int> delay;

				switch (weapon)
				{
					case UMP:
						x = { -1 * modifier, -4 * modifier, -2 * modifier, -4 * modifier, -9 * modifier, -3 * modifier, 11 * modifier, -4 * modifier, 9 * modifier, 18 * modifier, 15 * modifier, -1 * modifier, 5 * modifier, 0 * modifier, 9 * modifier, 5 * modifier, -12 * modifier, -19 * modifier, -1 * modifier, 15 * modifier, 17 * modifier, -6 * modifier, -20 * modifier, -3 * modifier, -3 * modifier };
						y = { 6 * modifier, 8 * modifier, 18 * modifier, 23 * modifier, 23 * modifier, 26 * modifier, 17 * modifier, 12 * modifier, 13 * modifier, 8 * modifier, 5 * modifier, 3 * modifier, 6 * modifier, 6 * modifier, -3 * modifier, -1 * modifier, 4 * modifier, 1 * modifier, -2 * modifier, -5 * modifier, -2 * modifier, 3 * modifier, -2 * modifier, -1 * modifier, -1 * modifier };
						delay = { 15, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 85, 90, 90, 85, 90, 90, 3000 };
						break;

					case M4A1:
						x = { 1 * modifier, 0 * modifier, -4 * modifier, 4 * modifier, -6 * modifier, -4 * modifier, 14 * modifier, 8 * modifier, 18 * modifier, -4 * modifier, -14 * modifier	, -25 * modifier	, -19 * modifier	, -22 * modifier	, 1 * modifier, 8 * modifier, -9 * modifier, -13 * modifier	, 3 * modifier, 1 * modifier };
						y = { 6 * modifier, 4 * modifier, 14 * modifier, 18 * modifier, 21 * modifier, 24 * modifier, 14 * modifier, 12 * modifier, 5 * modifier, 10 * modifier, 5 * modifier, -3 * modifier, 0 * modifier, -3 * modifier, 3 * modifier, 3 * modifier, 1 * modifier, -2 * modifier, 2 * modifier, 1 * modifier };
						delay = { 15, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 3000 };
						break;

					case M4A4:
						x = { 0, 2 * modifier, 0 * modifier, -6 * modifier, 7 * modifier, -9 * modifier, -5 * modifier, 16 * modifier, 11 * modifier, 22 * modifier, -4 * modifier, -18 * modifier, -30 * modifier, -24 * modifier, -25 * modifier, 0 * modifier, 8 * modifier, -11 * modifier, -13 * modifier, 2 * modifier, 33 * modifier, 10 * modifier, 27 * modifier, 10 * modifier, 11 * modifier, -12 * modifier, 6 * modifier, 4 * modifier, 3 * modifier, 4 * modifier };
						y = { 0, 7 * modifier, 9 * modifier, 16 * modifier, 21 * modifier, 23 * modifier, 27 * modifier, 15 * modifier, 13 * modifier, 5 * modifier, 11 * modifier, 6 * modifier, -4 * modifier, 0 * modifier, -6 * modifier, 4 * modifier, 4 * modifier, 1 * modifier, -2 * modifier, 2 * modifier, -1 * modifier, 6 * modifier, 3 * modifier, 2 * modifier, 0 * modifier, 0 * modifier, 5 * modifier, 5 * modifier, 1 * modifier, -1 * modifier };
						delay = { 15, 88, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 87, 87, 87, 87, 3000 };
						break;

					case AK47:
						x = { -4 * modifier, 4 * modifier, -3 * modifier, -1 * modifier, 13 * modifier, 8 * modifier, 13 * modifier, -17 * modifier, -42 * modifier, -21 * modifier, 12 * modifier, -15 * modifier, -26 * modifier, -3 * modifier, 40 * modifier, 19 * modifier, 14 * modifier, 27 * modifier, 33 * modifier, -21 * modifier, 7 * modifier, -7 * modifier, -8 * modifier, 19 * modifier, 5 * modifier, -20 * modifier, -33 * modifier, -45 * modifier, -14 * modifier, -14 * modifier };
						y = { 7 * modifier, 19 * modifier, 29 * modifier, 31 * modifier, 31 * modifier, 28 * modifier, 21 * modifier, 12 * modifier, -3 * modifier, 2 * modifier, 11 * modifier, 7 * modifier, -8 * modifier, 4 * modifier, 1 * modifier, 7 * modifier, 10 * modifier, 0 * modifier, -10 * modifier, -2 * modifier, 3 * modifier, 9 * modifier, 4 * modifier, -3 * modifier, 6 * modifier, -1 * modifier, -4 * modifier, -21 * modifier, 1 * modifier, 1 * modifier };
						delay = { 50, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 80, 3000 };
						break;

					case GALIL:
						x = { 4 * modifier, -2 * modifier, 6 * modifier, 12 * modifier, -1 * modifier, 2 * modifier, 6 * modifier, 11 * modifier, -4 * modifier, -22 * modifier, -30 * modifier, -29 * modifier, -9 * modifier, -12 * modifier, -7 * modifier, 0 * modifier, 4 * modifier, 25 * modifier, 14 * modifier, 25 * modifier, 31 * modifier, 6 * modifier, -12 * modifier, 13 * modifier, 10 * modifier, 16 * modifier, -9 * modifier, -32 * modifier, -24 * modifier, -15 * modifier, 6 * modifier, -14 * modifier, -24 * modifier, -13 * modifier, -13 * modifier };
						y = { 4 * modifier, 5 * modifier, 10 * modifier, 15 * modifier, 21 * modifier, 24 * modifier, 16 * modifier, 10 * modifier, 14 * modifier, 8 * modifier, -3 * modifier, -13 * modifier, 8 * modifier, 2 * modifier, 1 * modifier, 1 * modifier, 7 * modifier, 7 * modifier, 4 * modifier, -3 * modifier, -9 * modifier, 3 * modifier, 3 * modifier, -1 * modifier, -1 * modifier, -4 * modifier, 5 * modifier, -5 * modifier, -3 * modifier, 5 * modifier, 8 * modifier, -3 * modifier, -14 * modifier, -1 * modifier, -1 * modifier };
						delay = { 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 50, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 3000 };
						break;

					case FAMAS:
						x = { -4 * modifier, 1 * modifier, -6 * modifier, -1 * modifier, 0 * modifier, 14 * modifier, 16 * modifier, -6 * modifier, -20 * modifier, -16 * modifier, -13 * modifier, 4 * modifier, 23 * modifier, 12 * modifier, 20 * modifier, 5 * modifier, 15 * modifier, 3 * modifier, -4 * modifier, -25 * modifier, -3 * modifier, 11 * modifier, 15 * modifier, 15 * modifier, 15 * modifier };
						y = { 5 * modifier, 4 * modifier, 10 * modifier, 17 * modifier, 20 * modifier, 18 * modifier, 12 * modifier, 12 * modifier, 8 * modifier, 5 * modifier, 2 * modifier, 5 * modifier, 4 * modifier, 6 * modifier, -3 * modifier, 0 * modifier, 0 * modifier, 5 * modifier, 3 * modifier, -1 * modifier, 2 * modifier, 0 * modifier, -7 * modifier, -10 * modifier, -10 * modifier };
						delay = { 30, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 87, 88, 88, 88, 88, 88, 80, 88, 80, 84, 80, 88, 3000 };
						break;
				}

				for (size_t i = 0; i < x.size(); ++i)
				{
					if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						arduino.send_data("MOUSE_LEFT_HOLDED:" + to_string(x[i]) + "," + to_string(y[i]) + "," + to_string(delay[i]));
						sleep_for(milliseconds(delay[i]));
					}
				}
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				arduino.send_data("SPACE_BUTTON_HOLDED");
			}
			if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
			{
				arduino.send_data("MOUSE_MIDDLE_HOLDED");
			}
		}
	}
}
