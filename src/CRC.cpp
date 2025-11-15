#include "CRC.h"

std::uint16_t CRC::calculate(const std::uint8_t* data, std::size_t length) {
    std::uint16_t crc = 0x0000;
    constexpr std::uint16_t polynomial = 0x1021;

    for (std::size_t i = 0; i < length; ++i) {
        crc ^= static_cast<std::uint16_t>(data[i]) << 8;

        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x8000) {
                crc = static_cast<std::uint16_t>((crc << 1) ^ polynomial);
            } else {
                crc = static_cast<std::uint16_t>(crc << 1);
            }
        }
    }

    // No final XOR, no reflection
    return crc;
}
