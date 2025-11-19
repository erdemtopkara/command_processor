#pragma once

#include "Algorithm.h"
#include "SensorManager.h"
#include "CommandTypes.h"
#include "UartDriver_Stm32.hpp"
#include <cstdint>
#include <vector>

class SystemController {
public:
	void init();
    void run();    // infinite 10 ms loop (not used in CLI mode yet)

    // New: handle a single command packet and build response
    std::vector<std::uint8_t> handleCommandPacket(const std::vector<std::uint8_t>& packet);

private:
    Algorithm      algorithm;
    SensorManager  sensorManager;
    UartDriver_STM32 UartComm;

    std::uint32_t  sensorTimerMs;
    HealthStatus   healthStatus { 0x12345678 }; // dummy health value
};

extern uint8_t TimerFlagforSensorReadRequest;
