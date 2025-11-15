#pragma once

#include "Algorithm.h"
#include "SensorManager.h"
#include <cstdint>

class SystemController {
public:
    void init();
    void run();    // infinite 10 ms loop

private:
    Algorithm algorithm;
    SensorManager sensorManager;

    std::uint32_t sensorTimerMs;
};
