#pragma once

#include <cstdint>

enum class CommandId : std::uint8_t {
    FeedData   = 0xF1,
    HealthInfo = 0xF2
};

struct FeedDataCommand {
    float param1;
    float param2;
};

struct HealthQueryCommand {
    std::uint8_t param1;
};

struct HealthStatus {
    std::uint32_t value;
};

