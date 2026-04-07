#include "PacketCapture.h"

#include <set>

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

std::vector<std::string> PacketCapture::build_filter_patterns(const PacketFilter& f) const {
    std::vector<std::string> patterns;

    if (f.srcIp) {
        patterns.push_back("srcIp=" + f.srcIp->toString() + ";");
    }
    if (f.dstIp) {
        patterns.push_back("dstIp=" + f.dstIp->toString() + ";");
    }
    if (f.srcPort) {
        patterns.push_back("srcPort=" + std::to_string(f.srcPort.value()) + ";");
    }
    if (f.dstPort) {
        patterns.push_back("dstPort=" + std::to_string(f.dstPort.value()) + ";");
    }

    return patterns;
}

std::string PacketCapture::serialize_packet_event(const PacketEvent& e) const {
    return "srcIp=" + e.flow.srcIp.toString() + ";" +
        "dstIp=" + e.flow.dstIp.toString() + ";" +
        "srcPort=" + std::to_string(e.flow.srcPort) + ";" +
        "dstPort=" + std::to_string(e.flow.dstPort) + ";";
}

void PacketCapture::get_packet_metadata(PacketEvent& e, pcpp::IPv4Layer* ipLayer, pcpp::TcpLayer* tcpLayer) {
    e.flow.dstIp = ipLayer->getDstIPv4Address();
    e.flow.srcIp = ipLayer->getSrcIPv4Address();

    e.ipHeader.checksum   = ipLayer->getIPv4Header()->headerChecksum;
    e.ipHeader.timeToLive = ipLayer->getIPv4Header()->timeToLive;

    e.flow.srcPort = tcpLayer->getSrcPort();
    e.flow.dstPort = tcpLayer->getDstPort();
    e.tcpFlags     = this->get_tcp_flags(tcpLayer);

    const uint8_t* payload_data = tcpLayer->getLayerPayload();
    size_t payload_len           = tcpLayer->getLayerPayloadSize();
    if (payload_data && payload_len > 0) {
        e.payload.assign(payload_data, payload_data + payload_len);
    }
}

PacketEvent PacketCapture::cast_packet(pcpp::RawPacket* packet) {
    /*
     * Converts pcpp::RawPacket into a PacketEvent for our packet capture.
     * pcpp::RawPacket* is modified in pcpp::Packet therefore cannot be a constant reference.
     */
    PacketEvent event;

    pcpp::Packet parsed_packet(packet);
    event.length = static_cast<std::size_t>(packet->getRawDataLen());
    event.timestamp = std::chrono::system_clock::now();

    pcpp::IPv4Layer* ipLayer = parsed_packet.getLayerOfType<pcpp::IPv4Layer>();
    pcpp::TcpLayer* tcpLayer = parsed_packet.getLayerOfType<pcpp::TcpLayer>();
    if (ipLayer == nullptr || tcpLayer == nullptr) {
        return event;
    }

    this->get_packet_metadata(event, ipLayer, tcpLayer);

    return event;
}

std::optional<PacketEvent> PacketCapture::capture(pcpp::RawPacket* raw_packet) {
    PacketEvent packet = this->cast_packet(raw_packet);
    if (!this->packet_filter.has_value()) {
        return packet;
    }

    if (this->matches_pattern(packet, this->packet_filter.value())) {
        return packet;
    }

    return std::nullopt;
}

int PacketCapture::add_filter(PacketFilter packet_filter)
{
    this->packet_filter = packet_filter;

    const auto patterns = this->build_filter_patterns(packet_filter);
    if (patterns.empty()) {
        this->filter_matcher.reset();
    } else {
        this->filter_matcher.emplace(patterns);
    }

    return 0;
}

bool PacketCapture::matches_pattern(const PacketEvent& p, const PacketFilter& f) {
    const auto patterns = this->build_filter_patterns(f);
    if (patterns.empty()) {
        return true;
    }

    if (!this->filter_matcher.has_value()) {
        this->filter_matcher.emplace(patterns);
    }

    const std::string packet_text = this->serialize_packet_event(p);
    const auto matches = this->filter_matcher->search(packet_text);

    std::set<std::string> matched_tokens;
    for (const auto& m : matches) {
        matched_tokens.insert(m.val);
    }

    return matched_tokens.size() == patterns.size();
}
