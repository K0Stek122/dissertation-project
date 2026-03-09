#include <iostream>
#include <string>

#include "../Sniffer.h"
#include "../pcapplusplus/include/PcapLiveDeviceList.h"
#include "../pcapplusplus/include/PcapLiveDevice.h"
#include "../pcapplusplus/include/SystemUtils.h"
#include "../pcapplusplus/include/Packet.h"

#include "../PacketEvent.h"
#include "../Sniffer.h"
#include "../PacketCapture.h"

namespace Log {
    void LogFailure(const std::string& information ) {
        std::cout << "\033[31m[FAILURE] " << information << "\033[0m" << std::endl;
    }
    
    void LogSuccess(const std::string& information) {
        std::cout << "\033[32m[SUCCESS] " << information << "\033[0m" << std::endl;
    }
}

namespace TestCase {
    bool test_SnifferStart() {
        Sniffer sniffer;
        // Using this particular interface definitely WON'T cause problems...
        // fIx this later on.
        if (!sniffer.start(nullptr, "wlp0s20f3", "tcp")) {
            Log::LogFailure("test_SnifferStart");
            return false;
        } else {
            Log::LogSuccess("test_SnifferStart");
            return true;
        }
    }
    
    bool test_PCapturePacketCapture() {
        // MOCK PACKET CREATION
        uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
        pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), nullptr, false);
        
        PacketCapture p_capture;
        PacketEvent captured_packet = p_capture.capture("", mock_packet);
        
        if (captured_packet.flow.dstIp)
    }
};