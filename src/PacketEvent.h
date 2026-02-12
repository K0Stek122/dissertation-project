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

// This is packet metadata
struct Flow {
    pcpp::IPAddress srcIp;
    pcpp::IPAddress dstIp;
    uint16_t srcPort = 0;
    uint16_t dstPort = 0;
    pcpp::ProtocolType protocol = 0;
};

// This will be an event for packets instead of processing raw packets everywhere
struct PacketEvent {
    Flow flow;
    std::size_t length = 0;
    std::chrono::system_clock::time_point timestamp = {}; // This returns the system clock at which point the packet arrived.
};