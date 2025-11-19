#include "SystemController.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iomanip>

static std::vector<std::uint8_t> readHexPacketFromStdIn() {
    std::vector<std::uint8_t> packet;

    std::cout << "Enter packet bytes in hex (space separated), e.g.\n";
    std::cout << "0A 01 10 9A 64 4C 00 00 52 A8\n";
    std::cout << "> ";

    std::string line;
    if (!std::getline(std::cin, line)) {
        return packet;
    }

    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        std::uint32_t value = 0;
        std::stringstream converter;
        converter << std::hex << token;
        converter >> value;
        packet.push_back(static_cast<std::uint8_t>(value & 0xFF));
    }

    return packet;
}

static void printHexPacket(const std::vector<std::uint8_t>& packet) {
    if (packet.empty()) {
        std::cout << "No response." << std::endl;
        return;
    }

    std::cout << "Response packet: ";
    for (auto b : packet) {
        std::cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;
}

/*int main() {
    SystemController system;
    system.init();

    auto request = readHexPacketFromStdIn();
    if (request.empty()) {
        std::cout << "Empty input. Exiting." << std::endl;
        return 0;
    }

    auto response = system.handleCommandPacket(request);
    printHexPacket(response);

    return 0;
}*/
