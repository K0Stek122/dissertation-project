#include "Sniffer.h"

Sniffer::Sniffer() {

}

Sniffer::~Sniffer() {

}

bool Sniffer::start(pcpp::OnPacketArrivesCallback onPacketArrive, std::string device_name, std::string filter) {
    pcpp::PcapLiveDevice* dev;
    if (device_name.empty()) {
        auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
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
    pcap_dev->startCapture(onPacketArrive, nullptr);

    return true;
}

void Sniffer::stop() {
    pcap_dev->stopCapture();
    pcap_dev->close();
}