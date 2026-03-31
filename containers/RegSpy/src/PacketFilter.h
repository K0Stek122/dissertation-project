#pragma once

#include <string>
#include <optional>
// HEADER ONLY

struct PacketFilter {
    std::optional<pcpp::IPv4Address> srcIp;
    std::optional<pcpp::IPv4Address> dstIp;
    std::optional<uint16_t> dstPort;
    std::optional<uint16_t> srcPort;
};