#pragma once

#include <string>
#include <chrono>

#include "PacketEvent.h"

struct Alert {
    std::string rule_id;
    std::string severity;
    std::string matched_pattern;
    int match_offset = 0;
    Flow flow;
    std::string tcp_flags;
    std::chrono::system_clock::time_point timestamp;
};
