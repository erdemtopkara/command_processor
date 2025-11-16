#include "uart_ring.h"

extern "C" void App_10msTask(void)
{
    uint8_t byte;

    // Poll UART RX ring buffer
    while (UartRing_GetByte(&byte))
    {
        // TODO: Later this will feed CommandProcessor::feedByte(byte)
    }
}

extern "C" void App_45msTask(void)
{
    // TODO: 45 ms periodic task
}
