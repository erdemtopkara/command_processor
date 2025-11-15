#pragma once

#include <vector>
#include <cstdint>
#include <optional>
#include "CommandTypes.h"

class CommandParser {
public:
    struct Result {
        CommandId commandId;
        FeedDataCommand      feedData;
        HealthQueryCommand   healthQuery;
    };

    // Returns std::nullopt if packet is invalid (length, header, CRC, etc.)
    static std::optional<Result> parse(const std::vector<std::uint8_t>& packet);
};
