#pragma once

#include <string>
#include <functional>
#include <iostream>

#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/RawPacket.h"

class Sniffer {
private:
    pcpp::PcapLiveDevice* pcap_dev;

public:
    Sniffer();
    ~Sniffer();
    
    bool start(pcpp::OnPacketArrivesCallback onPacketArrive, std::string device_name, std::string filter);
    void stop();
};