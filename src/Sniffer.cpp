#include "Sniffer.h"
#include <iostream>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"

class Sniffer::Impl {
public:
    pcpp::PcapLiveDevice* dev = nullptr;
    Callback callback;

    static void onPacketArrive(pcpp::RawPacket* raw_packet, pcpp::PcapLiveDevice* dev, void* cookie) {
        auto* self = static_cast<Impl*>(cookie);
        const auto* data = raw_packet->getRawData();
        int len = raw_packet->getRawDataLen();
        auto timestamp = raw_packet->getPacketTimeStamp();
        self->callback(reinterpret_cast<const unsigned char*>(data), len, timestamp.tv_sec, timestamp.tv_nsec);
    }
};

bool Sniffer::open(const std::string& device_name) {
    this->impl_ = new Impl();
    impl_->dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(device_name);
    if (!impl_->dev) {
        std::cerr << "Error: No such device: " << device_name << std::endl;
        return false;
    }
    
    if (!impl_->dev->open()) {
        std::cerr << "Error: Cannot open the device: " << device_name << std::endl;
        return false;
    }
    return true;    
}

bool setFilter(const std::string& bpf_filter) {
    
}