#pragma once

#include <string>
// HEADER ONLY

struct PacketFilter {
    std::string srcIp = "";
    std::string dstIp = "";
    uint16_t dstPort = 0;
    uint16_t srcPort = 0;
};