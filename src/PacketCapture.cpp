#include "PacketCapture.h"

std::string PacketCapture::get_tcp_flags(pcpp::TcpLayer* tcpLayer) {
    std::string result = "";
    if (tcpLayer->getTcpHeader()->synFlag == 1)
        result += "SYN ";
    if (tcpLayer->getTcpHeader()->ackFlag == 1)
        result += "ACK ";
    if (tcpLayer->getTcpHeader()->pshFlag == 1)
        result += "PSH ";
    if (tcpLayer->getTcpHeader()->cwrFlag == 1)
        result += "CWR ";
    if (tcpLayer->getTcpHeader()->urgFlag == 1)
        result += "URG ";
    if (tcpLayer->getTcpHeader()->eceFlag == 1)
        result += "ECE ";
    if (tcpLayer->getTcpHeader()->rstFlag == 1)
        result += "RST ";
    if (tcpLayer->getTcpHeader()->finFlag == 1)
        result += "FIN ";

    return result;
}

PacketEvent PacketCapture::cast_packet(pcpp::RawPacket* packet) {
    PacketEvent event;
    
    pcpp::Packet parsed_packet(packet);
    
    pcpp::IPv4Layer* ipLayer = parsed_packet.getLayerOfType<pcpp::IPv4Layer>();
    if (ipLayer == NULL) {
        return event;
    }
    
    event.flow.dstIp = ipLayer->getDstIPv4Address();
    event.flow.srcIp = ipLayer->getSrcIPv4Address();

    event.ipHeader.checksum = ipLayer->getIPv4Header()->headerChecksum;
    event.ipHeader.timeToLive = ipLayer->getIPv4Header()->timeToLive;

    pcpp::TcpLayer* tcpLayer = parsed_packet.getLayerOfType<pcpp::TcpLayer>();
    if (tcpLayer == NULL) {
        return event;
    }
    
    event.flow.srcPort = tcpLayer->getSrcPort();
    event.flow.dstPort = tcpLayer->getDstPort();
    event.tcpFlags = this->get_tcp_flags(tcpLayer);
    
    return event;
}

std::optional<PacketEvent> PacketCapture::capture(pcpp::RawPacket* raw_packet) {
    PacketEvent packet = this->cast_packet(raw_packet);
    if (!this->packet_filter.has_value()) {
        this->packet_backlog.push_back(packet);
        return packet;
    }

    if (this->matches_pattern(packet, this->packet_filter.value())) {
        this->packet_backlog.push_back(packet);
        return packet;
    } else {
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

int PacketCapture::add_filter(PacketFilter packet_filter)
{
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