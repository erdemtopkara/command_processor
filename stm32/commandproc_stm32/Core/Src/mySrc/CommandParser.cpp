#include "CommandParser.h"
#include "CRC.h"
#include <cstring>   // std::memcpy

namespace {

// helpers for little-endian decoding
std::uint16_t readU16(const std::uint8_t* p) {
    return static_cast<std::uint16_t>(p[0])
         | (static_cast<std::uint16_t>(p[1]) << 8);
}

float readFloat(const std::uint8_t* p) {
    float value;
    std::memcpy(&value, p, sizeof(float));
    return value;
}

} // namespace

std::optional<CommandParser::Result>
CommandParser::parse(const std::vector<std::uint8_t>& packet)
{
    if (packet.size() < 5) {
        return std::nullopt; // too short
    }

    const std::uint8_t lengthByte = packet[0];
    if (lengthByte != packet.size()) {
        // Length byte should match actual packet size
        return std::nullopt;
    }

    const std::uint8_t header   = packet[1];
    const std::uint8_t command  = packet[2];

    // Last two bytes are CRC (LSB, MSB)
    if (packet.size() < 3 + 2) {
        return std::nullopt;
    }

    const std::size_t crcIndex = packet.size() - 2;
    const std::uint16_t receivedCrc = readU16(&packet[crcIndex]);

    // CRC over everything except the CRC bytes themselves
    const std::uint16_t computedCrc =
        CRC::calculate(packet.data(), packet.size() - 2);

    if (receivedCrc != computedCrc) {
        return std::nullopt;
    }

    Result result{};
    const std::uint8_t* payload = &packet[3];

    switch (static_cast<CommandId>(command)) {
    case CommandId::FeedData:
        // Expect:
        // length: 0x0D
        // header: 0x01
        if (header != 0x01 || packet.size() != 0x0D) {
            return std::nullopt;
        } else {
            // param1 (float), param2 (float) in little-endian
            result.commandId = CommandId::FeedData;
            result.feedData.param1 = readFloat(payload + 0);
            result.feedData.param2 = readFloat(payload + 4);
        }
        break;

    case CommandId::HealthInfo:
        // Expect:
        // length: 0x06
        // header: 0x01
        if (header != 0x01 || packet.size() != 0x06) {
            return std::nullopt;
        } else {
            // param1 is one byte at offset 0
            result.commandId = CommandId::HealthInfo;
            result.healthQuery.param1 = payload[0];
        }
        break;

    default:
        return std::nullopt;
    }

    return result;
}
