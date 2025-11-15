# Command Processor Project

This project implements a simple embedded-style command processor in C++.

The system simulates:
- A 10 ms main control loop
- Sensor readings every 45 ms
- A UART-based command interface
- A CUSTOM XMODEM CRC (0x1021) for packet integrity

## Build Instructions (macOS)

Requirements:
- g++ / clang++ with C++17 support

Build:

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o build_app
