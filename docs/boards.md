# Boards

## Getting Started Guide

### Connecting a new Arduino board

After connecting the board to your PC via USB cable, you should be able to check whether it's been recognized or not.

- Run `arduino-cli board list` to list all the connected and available boards.
- Identify the **Core** entry and install the required packages accordingly running `arduino-cli core install <core>`.
- To verify the core packages installation run `arduino-cli core list`.
