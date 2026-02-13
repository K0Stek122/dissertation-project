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

PacketEvent PacketCapture::capture(std::string filter, pcpp::RawPacket* raw_packet) {
    PacketEvent packet = this->cast_packet(raw_packet);
    for (auto filter : this->filters) {

    }
}

int PacketCapture::add_filter(PacketFilter packet_filter)
{
    this->filters.push_back(packet_filter);
    return 0;
}

std::vector<PacketFilter> PacketCapture::get_all_filters()
{
    return this->filters;
}

bool PacketCapture::remove_filter(int index)
{
    if (index >= 0 && index < (int)this->filters.size()) {
        this->filters.erase(this->filters.begin() + index);
        return true;
    }
    return false;
}
