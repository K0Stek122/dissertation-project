#include <optional>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"
#include "pcapplusplus/include/Packet.h"

#include "PacketFilter.h"
#include "PacketCapture.h"

class Sink {
private:
    PacketCapture p_capture;

private:
public:
    Sink();

    bool Run(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie);
};