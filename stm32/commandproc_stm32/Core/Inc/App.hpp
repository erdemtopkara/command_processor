#pragma once

#include <cstdint>

#include "UartDriver_Stm32.hpp"
#include "Algorithm.h"
#include "SystemController.h"
#include "CommandParser.h"

class App
{
public:
    App();

    void task10ms();
    void task45ms();

private:
    UartDriver_STM32 uart_;
    Algorithm        algorithm_;
    SystemController system_;
    CommandParser    processor_;
};
