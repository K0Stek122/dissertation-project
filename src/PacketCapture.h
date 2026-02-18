#pragma once

#include <string>
#include <vector>
#include <deque>
#include <iostream>

#include "pcapplusplus/include/IPv4Layer.h"
#include "pcapplusplus/include/TcpLayer.h"

#include "PacketEvent.h"
#include "PacketFilter.h"

class PacketCapture {
private:
    PacketEvent cast_packet(pcpp::RawPacket* packet);
    std::string get_tcp_flags(pcpp::TcpLayer* tcpLayer);
    std::optional<PacketFilter> packet_filter;
    std::deque<PacketEvent> packet_backlog;
    
    bool matches_pattern(const PacketEvent& e, const PacketFilter& f);

public:
    std::optional<PacketEvent> capture(std::string filter, pcpp::RawPacket* raw_packet);
    int add_filter(PacketFilter filter_type);
    
    bool process_packet_backlog();
};