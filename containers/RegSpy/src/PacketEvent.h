#pragma once

#include <string>
#include <cstdint>
#include <chrono>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"
#include "pcapplusplus/include/Packet.h"
#include "pcapplusplus/include/IPv4Layer.h"
#include "PacketEvent.h"

// HEADER ONLY

struct IPHeader {
    uint16_t checksum = 0;
    uint8_t timeToLive = 0;
};

// This is packet metadata
struct Flow {
    pcpp::IPv4Address srcIp;
    pcpp::IPv4Address dstIp;
    uint16_t srcPort = 0;
    uint16_t dstPort = 0;
    pcpp::ProtocolType protocol = 0;
};

// This will be an event for packets instead of processing raw packets everywhere
struct PacketEvent {
    Flow flow;
    IPHeader ipHeader;
    std::size_t length = 0;
    std::chrono::system_clock::time_point timestamp = {}; // This returns the system clock at which point the packet arrived.
    std::string tcpFlags = "";
};