# ArduinoStrike
ArduinoStrike is a program designed to control the recoil of weapons in **Counter-Strike 2** using **Arduino Leonardo** or any other compatible board. This tool simulates mouse movements for recoil control, implements rapid fire for semi-automatic weapons and adds bunny hop functionality. The software does not interact with the game client in any way, which makes it difficult to be detected by anti-cheat.

## Table of Contents
- [Introduction](#arduinostrike)
- [Key Features](#key-features)
- [Getting Started](#getting-started)
  - [Requirements](#requirements)
  - [Installation](#installation)
- [Configuration](#configuration)
- [View](#view)
- [Contributing](#contributing)
- [License](#license)

### Key Features
| Feature                   | Description                                                                                      |
|---------------------------|--------------------------------------------------------------------------------------------------|
| **Recoil Control**        | Reduces gun recoil by simulating controlled downward mouse movements for more accurate shots. |
| **Rapid Fire**            | Enhances the firing rate of semi-automatic weapons for faster shooting.                          |
| **Bunny Hop (Bhop)**      | Automatically performing jumps while holding down the spacebar provides quick movement in the game.     |
| **ColorBot (Color TriggerBot)** | Detects the color change on the screen when aiming with a rifle and fires the fastest possible shot without delay.        |
| **FastReload**            | Speeds up the reloading process of certain weapons in certain scenarios, allowing you to continue firing faster. |

### Additional Details
- **Note:** To enable the bhop feature, enter the command `bind mwheeldown +jump` in the game console.  
- **Important:** ArduinoStrike does not require a **USB Host Shield** - the board connects to a PC via USB and works as an HID device.

# Getting Started
### Requirements
- **Hardware**: Arduino Leonardo (or compatible board with native USB HID support)
- **Software**: Arduino IDE or PlatformIO, Counter-Strike 2

### Installation  

#### Option 1: Using PlatformIO (Recommended)  

1. **Clone the Repository**  
   ```bash  
   git clone https://github.com/DeniedAccessLife/ArduinoStrike.git  
   cd ArduinoStrike  
   ```  

2. **Upload the Firmware**  
   - Navigate to the `PlatformIO` folder:

     ```bash  
     cd PlatformIO  
     ```  
   - Run the `upload.bat` script for Windows.  
   - The script will automatically compile and upload the firmware to your connected Arduino board.  
   - Ensure the correct board and port are specified in the `platformio.ini` file.  

3. **Adding Support for Custom HID-Compatible Boards**  
   - If you plan to use a custom board with HID emulation, update the `platformio.ini` file located in the `PlatformIO` folder with the appropriate configurations for your board.  
   - Modify the `upload.bat` script in the same folder to support additional options.  

---

#### Option 2: Using Arduino IDE  

1. **Clone the Repository**  
   ```bash  
   git clone https://github.com/DeniedAccessLife/ArduinoStrike.git  
   cd ArduinoStrike  
   ```  

2. **Locate the Sketch File**  
   - The sketch file is located in the `PlatformIO/src` folder as `ArduinoStrike.cpp`.  

3. **Convert the Sketch for Arduino IDE**  
   - Rename `ArduinoStrike.cpp` to `ArduinoStrike.ino`.  
   - Create a new folder named `ArduinoStrike` in the `Arduino` projects directory on your system.  
   - Move the renamed file into the newly created `ArduinoStrike` folder.  

4. **Open the Project in Arduino IDE**  
   - Launch the Arduino IDE and open the newly created `ArduinoStrike.ino` file.  

5. **Connect and Configure Arduino**  
   - Connect your Arduino board to your computer.  
   - In the Arduino IDE, select the appropriate **Board** (e.g., `Arduino Leonardo`) and **Port**.  

6. **Upload the Firmware**  
   - Click the **Upload** button to flash the firmware onto your board.  

# Configuration
If the `Settings.cfg` file is not found when launching the program, you will be prompted to enter the following settings into the console:

| Setting               | Description                                                      | Value Type                 | Range/Options                    |
|-----------------------|------------------------------------------------------------------|----------------------------|----------------------------------|
| **Bhop**              | Enables or disables the bunny hop.                 | Integer                    | `1` (enable) / `0` (disable)     |
| **Rapid Fire**        | Enables or disables rapid fire.                                  | Integer                    | `1` (enable) / `0` (disable)     |
| **Sensitivity**       | Sets game sensitivity level.                                     | Integer                    | `1` to `8`                       |
| **Zoom Sensitivity**  | Adjusts sensitivity when zoomed in.                              | Decimal                    | `0.01` to `3.00`                 |
| **Recoil confirmation Key**  | Assigns a virtual key code to confirm recoil control or disables it.      | Integer                    | VK_CODE (enable) / `0` (disable) |
| **ColorBot Activation Key** | Sets a virtual key code to activate ColorBot or disables it. | Integer                  | VK_CODE (enable) / `0` (disable) |
| **ColorBot Threshold** | Sets the allowed color deviation threshold for ColorBot.       | Integer                     | `0` to `20`                      |
| **FastReload**        | Enables or disables fast reloading.                             | Integer                     | `1` (enable) / `0` (disable)     |

### Additional Details
- **ColorBot Threshold**: Higher values increase tolerance for color variations.
- **Sensitivity and Zoom Sensitivity**: Adjust these to match your gameplay preferences.
- **Virtual Key Codes (VK_CODE):** You can find VK_CODE values [here](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes) if you want to assign specific keys for activation.

These settings will be saved to `Settings.cfg` for future launches. You can edit this file to update the configuration as needed.

# View
Window of the program.

![alt text](https://raw.githubusercontent.com/DeniedAccessLife/ArduinoStrike/master/view.png)

# Contributing
If you want to contribute to this project:
- Request enhancements
- Report bugs and errors
- Create issues and pull requests
- Spread the word about this project

Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

# License
This project is licensed under the GNU License - see the [LICENSE](LICENSE) file for details.
