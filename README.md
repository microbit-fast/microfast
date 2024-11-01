# microfast

> [!WARNING]
> This library is still under development. Some things may change in future releases.

Fast and simple open-source library for programming BBC micro:bit V2 in C++.

## Features

- Lightweight and easy to use
- Supports basic functionalities for micro:bit V2
- Allows low-level hardware interaction, including direct register access

## Installation

> [!IMPORTANT]
> This library is designed for Linux systems only. Ensure you are using a compatible environment.

### Clone the repository

```bash
git clone https://github.com/microbit-fast/microfast.git
cd microfast
```
### Install necessary software

- Arch Linux:
    ```bash
     sudo pacman -Suy arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-binutils openocd
     ```
- Ubuntu/Debian:
    ```bash
    sudo apt update
    sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi libstdc++-arm-none-eabi-newlib 
    ```
- Other:  
    Install packages `arm-none-eabi` and `openocd`.

## Quick start

Open `main.cpp` in text editor. Here is an example of how to use the library:

```cpp
#include "microbit.h"                                                        // include the library 

int main() {
    microbit::init();                                                        // initialize the library
    while (1) {                                                              // main loop
        for (int i = 0; i < 1000; i++)                                       // loop for delay
            microbit::display::drawImage("happy");                           // draw smile on display
        for (int i = 0; i < 500; i++)
            microbit::display::drawImage("sad");
        while (microbit::pins::getDigitalValue(BUTTON_A)) {                  // while not button A pressed 
            microbit::display::drawImage("01100:10010:11110:10010:10010");   // draw "A" on display
        }
        microbit::sound::tone(440, 5000);                                    // play some tones
        microbit::sound::tone(430, 1000);
        microbit::sound::tone(420, 1000);
        microbit::sound::tone(410, 1000);
        microbit::sound::tone(400, 5000);
        microbit::serial::write("hello from your microbit :)");              // write string to the serial
        microbit::serial::write((string)microbit::random());                 // write random number to the serial
        for (int i = 4; i > 0; i--) {                                        // demonstrate how fast is it
            for (int j = 0; j < 20; j++) {
                for (int k = 0; k < i * 10; k++)
                    microbit::display::drawImage("square");
                for (int k = 0; k < i * 10; k++)
                    microbit::display::drawImage("small square");
            }
        }
    }
    return 0;
}
```

## Compilation
Simply call `make` to compile and flash the program to microbit.
