#include "PacketCapture.h"

std::string PacketCapture::get_tcp_flags(pcpp::TcpLayer* tcpLayer) {
    std::string result = "";

    if (tcpLayer->getTcpHeader()->synFlag == 1) result += "SYN ";
    if (tcpLayer->getTcpHeader()->ackFlag == 1) result += "ACK ";
    if (tcpLayer->getTcpHeader()->pshFlag == 1) result += "PSH ";
    if (tcpLayer->getTcpHeader()->cwrFlag == 1) result += "CWR ";
    if (tcpLayer->getTcpHeader()->urgFlag == 1) result += "URG ";
    if (tcpLayer->getTcpHeader()->eceFlag == 1) result += "ECE ";
    if (tcpLayer->getTcpHeader()->rstFlag == 1) result += "RST ";
    if (tcpLayer->getTcpHeader()->finFlag == 1) result += "FIN ";

    return result;
}

void PacketCapture::get_packet_metadata(PacketEvent& e, pcpp::IPv4Layer* ipLayer, pcpp::TcpLayer* tcpLayer) {
    e.flow.dstIp = ipLayer->getDstIPv4Address();
    e.flow.srcIp = ipLayer->getSrcIPv4Address();

    e.ipHeader.checksum = ipLayer->getIPv4Header()->headerChecksum;
    e.ipHeader.timeToLive = ipLayer->getIPv4Header()->timeToLive;

    e.flow.srcPort = tcpLayer->getSrcPort();
    e.flow.dstPort = tcpLayer->getDstPort();
    e.tcpFlags = this->get_tcp_flags(tcpLayer);
    
    
}

PacketEvent PacketCapture::cast_packet(pcpp::RawPacket* packet) {
    /*
     * Converts pcpp::RawPacket into a PacketEvent for our packet capture.
     * pcpp::RawPacket* is modified in pcpp::Packet therefore cannot be a constant reference.
     */
    PacketEvent event;
    
    pcpp::Packet parsed_packet(packet);
    
    pcpp::IPv4Layer* ipLayer = parsed_packet.getLayerOfType<pcpp::IPv4Layer>();
    pcpp::TcpLayer* tcpLayer = parsed_packet.getLayerOfType<pcpp::TcpLayer>();
    if (ipLayer == NULL || tcpLayer == NULL) { // Packet is invalid. Need to return an empty event.
        return event;
    }
    
    event.data.payload = std::vector<uint8_t>(packet->getRawData(), packet->getRawData() + packet->getRawDataLen());
    event.data.len = packet->getRawDataLen();

    this->get_packet_metadata(event, ipLayer, tcpLayer);
    
    return event;
}

std::optional<PacketEvent> PacketCapture::capture(pcpp::RawPacket* raw_packet) {
    PacketEvent packet = this->cast_packet(raw_packet);
    
    if (this->packet_filters.empty()) {
        this->packet_backlog.push_back(packet);
        return packet;
    } else {
        for (const auto& filter : this->packet_filters) {
            if (this->matches_pattern(packet, filter)) {
                this->packet_backlog.push_back(packet);
                return packet;
            }
        }
        return std::nullopt;
    }
    
}

bool PacketCapture::process_packet_backlog() {
    while (!this->packet_backlog.empty()) {
        PacketEvent current_packet_event = this->packet_backlog.front();
        this->packet_backlog.pop_front();
        if (this->matches_pattern(current_packet_event, this->packet_filter.value())) {
            //DO stuff to the packet here.
            return true;
        }
    }
    return false;
}

void PacketCapture::print_packet_data(const PacketEvent &p) {
    for (const auto& byte : p.data.payload) {
        std::cout << std::hex << std::to_string(byte) << " ";
    }
    std::cout << std::endl;
}

int PacketCapture::add_filter(PacketFilter packet_filter) {
    this->packet_filters.push_back(packet_filter);
    this->packet_filter = packet_filter;
    return 0;
}

bool PacketCapture::matches_pattern(const PacketEvent& p, const PacketFilter& f) {
    if (f.srcIp && p.flow.srcIp.toInt() != f.srcIp.value().toInt()) return false;
    if (f.dstIp && p.flow.dstIp.toInt() != f.dstIp.value().toInt()) return false;
    if (f.srcPort && p.flow.srcPort != f.srcPort.value()) return false;
    if (f.dstPort && p.flow.dstPort != f.dstPort.value()) return false;
    return true;
}