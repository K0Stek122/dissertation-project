#pragma once

#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/RawPacket.h"

#include "PacketCapture.h"
#include "Detector.h"
#include "OutputManager.h"

class Sink {
private:
    PacketCapture p_capture;
    Detector& detector;
    OutputManager& output;

public:
    Sink(Detector& detector, OutputManager& output);

    bool Run(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie);
};
