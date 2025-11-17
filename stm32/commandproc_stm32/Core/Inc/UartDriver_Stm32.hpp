#ifndef UARTDRIVER_STM32_HPP
#define UARTDRIVER_STM32_HPP

#include <cstdint>
#include <cstddef>

class UartDriver_STM32
{
public:
    UartDriver_STM32() = default;
    ~UartDriver_STM32() = default;

    // Sends a buffer over UART (blocking transmit for now)
    void send(const std::uint8_t* data, std::size_t length);

    // Tries to read one byte from RX ring buffer
    // Returns true if a byte is read, false if buffer is empty
    bool receive(std::uint8_t& byte);
};

#endif // UARTDRIVER_STM32_HPP
