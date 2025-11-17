#include "App.hpp"

App::App()
    : uart_(),
      algorithm_(),
      system_(),
      processor_()
{
}

void App::task10ms()
{
    uint8_t byte = 0;

    // Read available bytes from the UART ring buffer
    while (uart_.receive(byte))
    {
        // Forward the received byte to the command parser
        // processor_.feedByte(byte);
    }

    // Example algorithm execution (if needed later)
    // algorithm_.run();
}

void App::task45ms()
{
    // Example periodic system operations
    // system_.update();
}
