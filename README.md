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
```

## Architecture Overview

### SystemController
- Owns:
  - `Algorithm`
  - `SensorManager`
- Runs the **10 ms main loop**
- Schedules **sensor reads every 45 ms**
- Processes parsed commands
- Builds response packets

### Algorithm
- Represents the periodic control algorithm.
- `feed(param1, param2)` stores new data coming from F1 packets.
- `run()` is executed every 10 ms (simulated workload).

### SensorManager
- Simulates a sensor read operation.
- Prints a message every 45 ms.
- Can later be replaced by real SPI/I2C drivers on STM32.

### CommandParser
- Validates packet structure:
  - Length  
  - Header  
  - Command ID  
  - CRC  
- Extracts payload into typed structures:
  - `FeedDataCommand`
  - `HealthQueryCommand`

### CRC
- Implements CUSTOM XMODEM CRC:
  - Polynomial: `0x1021`
  - Initial value: `0x0000`
  - Final XOR: `0x0000`
  - Input/Output reflection: **false**

### main.cpp
- Provides a simple UART simulation:
  - Reads a hex-encoded packet from stdin
  - Passes it to `SystemController::handleCommandPacket()`
  - Prints the response as hex

---

## Command Definitions

### 1. Feed Data Command (F1)

#### Request Format
| Byte | Description |
|------|-------------|
| 0 | Length = `0x0D` |
| 1 | Header = `0x01` |
| 2 | Command ID = `0xF1` |
| 3–6 | `float param1` (little-endian) |
| 7–10 | `float param2` (little-endian) |
| 11–12 | CRC (LSB, MSB) |

#### Response Format
| Byte | Description |
|------|-------------|
| 0 | Length = `0x06` |
| 1 | Header = `0x02` |
| 2 | Command ID = `0xF1` |
| 3 | Status (`0x00` = success) |
| 4–5 | CRC |

---

### 2. Health Query Command (F2)

#### Request Format
| Byte | Description |
|------|-------------|
| 0 | Length = `0x06` |
| 1 | Header = `0x01` |
| 2 | Command ID = `0xF2` |
| 3 | `param1` → `0xAB` = send health status |
| 4–5 | CRC |

#### Response Format
| Byte | Description |
|------|-------------|
| 0 | Length = `0x0B` |
| 1 | Header = `0x02` |
| 2 | Command ID = `0xF2` |
| 3–6 | `uint32_t healthStatus` (little-endian) |
| 7–8 | CRC |

---

## Test Scenarios

### 1. F1 FeedData happy path
**Build a packet with:**
- Length = `0x0D`
- Header = `0x01`
- Command = `0xF1`
- Two valid floats
- Correct CRC

**Expected:**
- Parser accepts the packet  
- Algorithm receives parameters  
- Response contains `status = 0x00`

---

### 2. F2 HealthQuery with param1 = 0xAB
**Valid F2 request**

**Expected:**
- Response length = `0x0B`
- Health status = `0x12345678` (default)
- Valid CRC

---

### 3. Invalid CRC
- Flip any bit in request

**Expected:**
- Parser returns `nullopt`
- System produces **no response**

---

## Porting to STM32

This architecture is fully portable.  
On STM32:

- Replace `SystemController::run()` with a hardware timer interrupt
- Replace UART simulation with HAL UART RX/TX callbacks
- Replace `SensorManager` mock with real ADC/SPI/I2C drivers

Core logic and packet processing remain unchanged.

---

## License
This project is intended as an educational exercise and has no associated license restrictions unless required by the assignment.

## Running Provided Test Cases

Example:

```bash
cat tests/feed_f1_valid.txt | ./build_app

