#pragma once

#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <optional>

#include "AhoCorasickDFA.h"

#include "pcapplusplus/include/IPv4Layer.h"
#include "pcapplusplus/include/TcpLayer.h"

#include "PacketEvent.h"
#include "PacketFilter.h"

class PacketCapture {
// VARIABLES
private:
    std::optional<PacketFilter> packet_filter;
    std::optional<AhoCorasick> filter_matcher;
    std::deque<PacketEvent> packet_backlog;

// METHODS
private:
    PacketEvent cast_packet(pcpp::RawPacket* packet);
    std::string get_tcp_flags(pcpp::TcpLayer* tcpLayer);
    bool matches_pattern(const PacketEvent& e, const PacketFilter& f); // Matches a PacketEvent with a PacketFilter.
    void get_packet_metadata(PacketEvent& e, pcpp::IPv4Layer* ipLayer, pcpp::TcpLayer* tcpLayer);
    std::vector<std::string> build_filter_patterns(const PacketFilter& f) const;
    std::string serialize_packet_event(const PacketEvent& e) const;

public:
    std::optional<PacketEvent> capture(pcpp::RawPacket* raw_packet);
    int add_filter(PacketFilter filter_type);
    bool process_packet_backlog();
};