#include "Sniffer.h"

Sniffer::Sniffer() {

}

Sniffer::~Sniffer() {

}

bool Sniffer::start(
    pcpp::OnPacketArrivesCallback onPacketArrive,
    std::string device_name,
    std::string filter,
    void* cookie
) {
    pcpp::PcapLiveDevice* dev;
    if (device_name.empty()) {
        auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
        if (devices.empty()) {
            return false;
        }
        dev = pcpp::PcapLiveDeviceList::getInstance().getDeviceByName(devices[0]->getName());
        
    } else {
        dev = pcpp::PcapLiveDeviceList::getInstance().getDeviceByName(device_name);
    }
    
    
    if (dev == nullptr) {
        return false;
    }
    
    this->pcap_dev = dev;
    if (!this->pcap_dev->open()) {
        return false;
    }
    
    pcap_dev->setFilter(filter);
    if (!pcap_dev->startCapture(onPacketArrive, cookie)) {
        return false;
    }

    return true;
}

void Sniffer::stop() {
    pcap_dev->stopCapture();
    pcap_dev->close();
}
