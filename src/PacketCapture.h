#pragma once

#include "pcapplusplus/include/IPv4Layer.h"
#include "pcapplusplus/include/TcpLayer.h"

#include "PacketEvent.h"

class PacketCapture {
private:
    PacketEvent cast_packet(pcpp::RawPacket* packet);
    std::string get_tcp_flags(pcpp::TcpLayer* tcpLayer);

public:
    PacketEvent capture(std::string filter, pcpp::RawPacket* raw_packet);
};