#include "SystemController.h"
#include <thread>
#include <chrono>

void SystemController::init() {
    // TODO: initialize components
}

void SystemController::run() {
    while (true) {
        algorithm.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
