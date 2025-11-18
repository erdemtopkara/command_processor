#include "App.hpp"

App::App()
    : uart_()
    , algorithm_()
    , system_()
    , processor_()
{
    // Nothing special to do here for now
}

void App::task10ms()
{
    std::uint8_t byte = 0u;

    // Drain all bytes from UART RX ring and feed the command parser
    while (uart_.receive(byte))
    {
        // Optional echo for quick UART sanity check
        uart_.send(&byte, 1u);
    }
}

void App::task45ms()
{
    // Reserved for 45 ms periodic work (sensors, algorithm, etc.)
    // Example for later:
    // system_.update();
    // algorithm_.run();
}
