#pragma once

#include <cstddef>
#include <cstdint>

class CRC {
public:
    // CUSTOM XMODEM
    // Polynomial:      0x1021
    // Initial value:   0x0000
    // Final XOR:       0x0000
    // Input/Output reflection: false
    static std::uint16_t calculate(const std::uint8_t* data, std::size_t length);
};
