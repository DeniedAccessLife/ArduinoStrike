# ArduinoStrike
ArduinoStrike is a program designed to control weapon recoil in **Counter-Strike 2** using an **Arduino Leonardo** or any other compatible board. This tool simulates mouse movements to manage recoil, implements rapid fire, and adds bunny hop functionality without directly interacting with the game client, making detection more challenging.

### Key Features
- **Recoil Control**: Reduces weapon recoil by simulating downward mouse movements.
- **Rapid Fire**: Increases firing rate for semi-automatic weapons.
- **Bunny Hop (Bhop)**: Automatically performs jumps when holding down the space bar for easier movement.

> Ensure you use this command in the game console to enable the bhop feature: `bind mwheeldown +jump;`.

### Important
The ArduinoStrike software **does not require a USB Host Shield**. Arduino connects to the PC via USB and functions as an HID device.

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
After the first launch `ArduinoStrike.exe`, you will be prompted to enter values for various settings:

- **Bhop**: Enter `1` to enable bunny hop or `0` to disable.
- **Rapid Fire**: Enter `1` to enable rapid fire or `0` to disable.
- **Sensitivity**: Enter an integer between `1` and `8` for general sensitivity.
- **Zoom Sensitivity**: Enter a decimal value between `0.01` and `3.00` for zoom sensitivity.
- **Confirmation Key**: Enter the virtual key code (`VK_CODE`) for the recoil control confirmation key, or `0` to disable this feature.
- **ColorBot Activation Key**: Enter the virtual key code (`VK_CODE`) for the ColorBot activation key, or `0` to disable ColorBot activation.
- **ColorBot Threshold**: Enter a value between `0` and `20` to set the allowable color deviation for ColorBot activation.

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
