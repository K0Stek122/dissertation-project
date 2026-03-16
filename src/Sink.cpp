#include "Sink.h"

Sink::Sink(PacketFilter &p_filter, PacketCapture &p_capture) {
    this->p_filter = p_filter;
    this->p_capture = p_capture;
}

bool Sink::Run(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie) {
    std::optional<PacketEvent> captured_packet = this->p_capture.capture(packet);
    if (!captured_packet.has_value()) {
        return false;
    }
    
    std::cout << captured_packet.value().flow.dstIp << std::endl;

    return true;
}
