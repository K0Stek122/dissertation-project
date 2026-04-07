#include "Sink.h"

Sink::Sink(Detector& detector, OutputManager& output)
    : detector(detector), output(output)
{
}

bool Sink::Run(pcpp::RawPacket* packet, [[maybe_unused]] pcpp::PcapLiveDevice* device, [[maybe_unused]] void* cookie) {
    std::optional<PacketEvent> captured = this->p_capture.capture(packet);
    if (!captured.has_value()) {
        return false;
    }

    const std::vector<Alert> alerts = this->detector.detect(captured.value());
    for (const auto& alert : alerts) {
        this->output.emit(alert);
    }

    return true;
}
