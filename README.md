
# Chip-8 Emulator

This project is a Chip-8 emulator written in C++. It is designed to provide an efficient and accurate emulation of the Chip-8 platform, and can be used for running various classic games like Pong and Space Invaders.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Compilation](#compilation)
- [Usage](#usage)
- [Contributing](#contributing)

## Features

- Accurate emulation of the Chip-8 platform
- Runs a variety of classic games
- Integrated test suite for easy debugging
- Simple and clean C++ implementation

## Getting Started

These instructions will help you set up and run the Chip-8 emulator on your local machine.

### Prerequisites

To compile and run this project, you will need a C++ compiler with C++14 support. The following are some examples of compatible compilers:

- GCC 5.0 or later
- Clang 3.4 or later
- Microsoft Visual Studio 2015 or later

### Compilation

1. Clone the repository to your local machine:

```bash
git clone https://github.com/yourusername/chip8-emulator.git
cd chip8-emulator/src
````
2. Compile using this command
```bash
clang++ -std=c++11 main.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -o main
```

### Usage

1. Run with the desired Scale, Delay & ROM (Preferred settings: Scale=10, Delay=15):
```bash
./main 10 15 roms/space_invaders.ch8
```

### Contributing
Feel free to submit issues and pull requests for bug fixes, improvements, or new features. Any contributions are greatly appreciated!
