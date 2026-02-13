#pragma once

#include <string>
#include <vector>

#include "pcapplusplus/include/IPv4Layer.h"
#include "pcapplusplus/include/TcpLayer.h"

#include "PacketEvent.h"
#include "FilterType.h"

class PacketCapture {
private:
    PacketEvent cast_packet(pcpp::RawPacket* packet);
    std::string get_tcp_flags(pcpp::TcpLayer* tcpLayer);
    std::vector<FilterType> filters;

public:
    PacketEvent capture(std::string filter, pcpp::RawPacket* raw_packet);
    int add_filter(FilterType filter_type);
    std::vector<FilterType> get_all_filters();
    bool remove_filter(int index);
};