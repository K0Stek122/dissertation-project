#pragma once

#include <string>
#include <cstdint>
#include <chrono>

// HEADER ONLY

enum class LAYER4_PROTOCOL {
    TCP,
    UDP,
    OTHER
};

// This is packet metadata
struct Flow {
    std::string srcIp;
    std::string dstIp;
    uint16_t srcPort = 0;
    uint16_t dstPort = 0;
    LAYER4_PROTOCOL protocol = LAYER4_PROTOCOL::OTHER;
};

// This will be an event for packets instead of processing raw packets everywhere
struct PacketEvent {
    Flow flow;
    std::size_t length = 0;
    std::chrono::system_clock::time_point timestamp = {}; // This returns the system clock at which point the packet arrived.
};