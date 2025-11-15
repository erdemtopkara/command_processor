#include "SystemController.h"
#include <thread>
#include <chrono>

void SystemController::init() {
    sensorTimerMs = 0;
    // TODO: initialize other components if needed
}

void SystemController::run() {
    using namespace std::chrono;

    const std::uint32_t mainLoopPeriodMs = 10;
    const std::uint32_t sensorPeriodMs   = 45;

    while (true) {
        auto loopStart = steady_clock::now();

        // 10 ms algorithm task
        algorithm.run();

        // Sensor scheduling: every ~45 ms
        sensorTimerMs += mainLoopPeriodMs;
        if (sensorTimerMs >= sensorPeriodMs) {
            sensorManager.readSensors();
            sensorTimerMs = 0;
        }

        auto loopEnd   = steady_clock::now();
        auto elapsedMs = duration_cast<milliseconds>(loopEnd - loopStart).count();

        if (elapsedMs < mainLoopPeriodMs) {
            std::this_thread::sleep_for(milliseconds(mainLoopPeriodMs - elapsedMs));
        }
    }
}
