# ArduinoStrike
ArduinoStrike is a program designed to control weapon recoil in **Counter-Strike 2** using an **Arduino Leonardo** or any other compatible board. This tool simulates mouse movements to manage recoil, implements rapid fire, and adds bunny hop functionality without directly interacting with the game client, making detection more challenging.

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
| **Recoil Control**        | Reduces weapon recoil by simulating controlled downward mouse movements for more accurate shots. |
| **Rapid Fire**            | Enhances the firing rate of semi-automatic weapons for faster shooting.                          |
| **Bunny Hop (Bhop)**      | Automatically performs jumps by holding down the space bar, enabling fluid movement in-game.     |
| **ColorBot (Color TriggerBot)** | Detects specified color changes on-screen to trigger automatic shooting actions.        |
| **FastReload**            | Speeds up the reload process for certain weapons in specific scenarios, allowing quicker return to action. |

### Additional Details
- **Note:** To enable the bhop feature, enter the command `bind mwheeldown +jump` in the game console.  
- **Important:** ArduinoStrike **does not require a USB Host Shield**—it connects to the PC via USB and functions as an HID device.

# Getting Started
### Requirements
- **Hardware**: Arduino Leonardo (or compatible board with native USB HID support)
- **Software**: Arduino IDE, Counter-Strike 2

### Installation
1. **Clone the Repository**  
```bash
git clone https://github.com/DeniedAccessLife/ArduinoStrike.git
cd ArduinoStrike
```

2. **Open the Project in Arduino IDE**  
Open the `ArduinoStrike.ino` file located in the `ArduinoSketch` folder in the repository.

3. **Connect and Configure Arduino**  
Connect Arduino board to your computer. In the Arduino IDE, select the appropriate **Board** (`Arduino Leonardo`) and **Port**.

4. **Upload the Firmware**  
Click the **Upload** button in the Arduino IDE to flash the `ArduinoStrike` firmware to your board. Once uploaded, your Arduino will function as an HID device, controlling mouse movements according to the code logic.

# Configuration
If the `Settings.cfg` file is not found when launching the program, you will be prompted to enter the following settings into the console:

| Setting               | Description                                                      | Value Type                 | Range/Options                    |
|-----------------------|------------------------------------------------------------------|----------------------------|----------------------------------|
| **Bhop**              | Enables or disables the bunny hop functionality.                 | Integer                    | `1` (enable) / `0` (disable)     |
| **Rapid Fire**        | Enables or disables rapid fire.                                  | Integer                    | `1` (enable) / `0` (disable)     |
| **Sensitivity**       | Sets game sensitivity level.                                     | Integer                    | `1` to `8`                       |
| **Zoom Sensitivity**  | Adjusts sensitivity when zoomed in.                              | Decimal                    | `0.01` to `3.00`                 |
| **Confirmation Key**  | Assigns a virtual key code for confirmation or disables it.      | Integer                    | VK_CODE (enable) / `0` (disable) |
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
