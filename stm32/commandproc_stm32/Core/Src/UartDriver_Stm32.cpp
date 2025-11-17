#include "UartDriver_Stm32.hpp"

extern "C" {
#include "main.h"
#include "uart_ring.h"
}

extern UART_HandleTypeDef huart2;

void UartDriver_STM32::send(const std::uint8_t* data, std::size_t length)
{
    if ((data == nullptr) || (length == 0u))
    {
        return;
    }

    // Blocking transmit is acceptable for this project scope
    (void)HAL_UART_Transmit(&huart2,
                            const_cast<std::uint8_t*>(data),
                            static_cast<std::uint16_t>(length),
                            HAL_MAX_DELAY);
}

bool UartDriver_STM32::receive(std::uint8_t& byte)
{
    // Wrapper around C ring buffer API
    return (UartRing_GetByte(&byte) == 1);
}
