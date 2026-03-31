#include "Sink.h"

Sink::Sink() {
    PacketFilter p_filter;
    p_filter.dstIp = pcpp::IPv4Address("151.101.1.91");
    this->p_capture.add_filter(p_filter);
}

bool Sink::Run(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie) {
    std::optional<PacketEvent> captured_packet = this->p_capture.capture(packet);
    if (!captured_packet.has_value()) {
        return false;
    }
    
    std::cout << captured_packet.value().flow.dstIp << std::endl;

    return true;
}
