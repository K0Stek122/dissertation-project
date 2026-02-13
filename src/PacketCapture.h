#pragma once

#include <string>
#include <vector>

#include "pcapplusplus/include/IPv4Layer.h"
#include "pcapplusplus/include/TcpLayer.h"

#include "PacketEvent.h"
#include "PacketFilter.h"

class PacketCapture {
private:
    PacketEvent cast_packet(pcpp::RawPacket* packet);
    std::string get_tcp_flags(pcpp::TcpLayer* tcpLayer);
    std::vector<PacketFilter> filters;
    std::vector<PacketEvent> packet_backlog;

public:
    PacketEvent capture(std::string filter, pcpp::RawPacket* raw_packet);
    int add_filter(PacketFilter filter_type);
    std::vector<PacketFilter> get_all_filters();
    bool remove_filter(int index);
    
    bool is_filtered(PacketEvent packet_event);
    bool process_packet_backlog();
};