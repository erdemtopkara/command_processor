#include "SystemController.h"
#include "CommandParser.h"
#include "CRC.h"
#include <thread>
#include <chrono>
#include <iostream>

uint8_t TimerFlagforSensorReadRequest;
uint8_t UartReceiveFlagRequest;
void SystemController::init() {

}

void SystemController::run() {
    using namespace std::chrono;
    const std::uint32_t mainLoopPeriodMs = 10;

    while (true) {
        auto loopStart = steady_clock::now();

        // Sensor scheduling: every ~45 ms
        if (TimerFlagforSensorReadRequest == 1U)
        {
            sensorManager.readSensors();
            TimerFlagforSensorReadRequest = 0U;
        }

        //uart command handler
        if (UartReceiveFlagRequest == 1)
        {
        	uint8_t length = 0;
        	//first byte length
        	UartComm.receive(length);
            std::uint8_t ReceiveRxData[length];
            ReceiveRxData[0] = length;
            for (uint8_t i = 1; i < length; i++)
            {
                UartComm.receive(ReceiveRxData[i]);
            }
            std::vector<std::uint8_t> packet(ReceiveRxData, ReceiveRxData + length); // casting
            auto response = handleCommandPacket(packet);
            UartComm.send(response.data(), response.size()); // transmit uart
            UartReceiveFlagRequest = 0;
        }
        auto loopEnd   = steady_clock::now();
        auto elapsedMs = duration_cast<milliseconds>(loopEnd - loopStart).count();

        if (elapsedMs < mainLoopPeriodMs) {
            std::this_thread::sleep_for(milliseconds(mainLoopPeriodMs - elapsedMs));
        }
    }
}

std::vector<std::uint8_t>
SystemController::handleCommandPacket(const std::vector<std::uint8_t>& packet)
{
    std::vector<std::uint8_t> response;

    auto parsed = CommandParser::parse(packet);
    if (!parsed.has_value()) {
        std::cerr << "[CMD] Invalid packet" << std::endl;
        return response;
    }

    const auto& result = parsed.value();

    switch (result.commandId) {
    case CommandId::FeedData: {
        // Feed algorithm with incoming parameters
        algorithm.feed(result.feedData.param1, result.feedData.param2);

        // Build response:
        // Length (0x06), Header (0x02), Command (0xF1),
        // status (0x00 = OK), CRC (LSB, MSB)
        response.push_back(0x06);     // length
        response.push_back(0x02);     // header for response
        response.push_back(0xF1);     // command id
        response.push_back(0x00);     // status = success

        std::uint16_t crc = CRC::calculate(response.data(), response.size());
        response.push_back(static_cast<std::uint8_t>(crc & 0x00FF));        // CRC LSB
        response.push_back(static_cast<std::uint8_t>((crc >> 8) & 0x00FF)); // CRC MSB
        break;
    }

    case CommandId::HealthInfo: {
        // Only respond if param1 == 0xAB
        if (result.healthQuery.param1 != 0xAB) {
            std::cerr << "[CMD] Health query ignored (param1 != 0xAB)" << std::endl;
            return response;
        }

        // Build response:
        // Length (0x0B), Header (0x02), Command (0xF2),
        // healthStatus (uint32, little-endian), CRC (LSB, MSB)
        response.push_back(0x0B);     // length
        response.push_back(0x02);     // header for response
        response.push_back(0xF2);     // command id

        std::uint32_t v = healthStatus.value;
        response.push_back(static_cast<std::uint8_t>(v & 0x000000FF));
        response.push_back(static_cast<std::uint8_t>((v >> 8)  & 0x000000FF));
        response.push_back(static_cast<std::uint8_t>((v >> 16) & 0x000000FF));
        response.push_back(static_cast<std::uint8_t>((v >> 24) & 0x000000FF));

        std::uint16_t crc = CRC::calculate(response.data(), response.size());
        response.push_back(static_cast<std::uint8_t>(crc & 0x00FF));        // CRC LSB
        response.push_back(static_cast<std::uint8_t>((crc >> 8) & 0x00FF)); // CRC MSB
        break;
    }

    default:
        std::cerr << "[CMD] Unsupported command" << std::endl;
        break;
    }

    return response;
}

