# Arduino CLI

## Getting Started Guide

### Intro

This **Getting Started Guide** was extracted from the original source at [docs.arduino.cc](https://docs.arduino.cc/arduino-cli/).  
[Arduino CLI](https://docs.arduino.cc/arduino-cli/) provides all the features you can find in the [Arduino IDE](https://www.arduino.cc/en/software).  
The `arduino-cli` utility is a container of commands and each command has its own dedicated help text that can be shown with the `help` command.

### Requirements

The install script requires `sh`, which is always available on **Linux** and **macOS**.  
The `sh` utility is not available by default on **Windows**, though it is available as part of [Git for Windows](https://gitforwindows.org/).

### Installation

- Run `curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh` to install the latest version of Arduino CLI.
- Run `arduino-cli config init` to create the configuration files.
- Run `arduino-cli core update-index` to update the local cache of available platforms and libraries.

### (Windows Only) - Arduino CLI - WSL USB Forwarding

Before attaching your USB device, ensure that a **WSL command line** is open.  
This will keep the **WSL 2 lightweight VM** active.

- On **Powershell** run `usbipd list` and identify the **bus_id** to forward.
- On **Powershell** run `usbipd bind --busid <bus_id>` to share the device.
- On **Powershell** run `usbipd attach --wsl --busid <bus_id>` to attach the shared device to WSL.
- On **Bash** run `lsusb` to verify that the shared device is visible.

### Connecting a new Arduino board

After connecting the board to your PC via USB cable, you should be able to check whether it's been recognized or not.

- Run `arduino-cli board list` to list all the connected and available boards.
- Identify the **Core** entry and install the required packages accordingly running `arduino-cli core install <core>`.
- To verify the core packages installation run `arduino-cli core list`.
