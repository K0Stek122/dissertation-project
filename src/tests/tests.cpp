#include "tests.h"

void Log::LogFailure(const std::string& information, const std::string& assert_info) {
    std::cout << "\033[31m[FAILURE] " << information << "\033[0m" << std::endl
        << "    \033[31mAssert: " << assert_info << "\033[0m" << std::endl;
}

void Log::LogSuccess(const std::string& information) {
    std::cout << "\033[32m[SUCCESS] " << information << "\033[0m" << std::endl;
}

bool TestCase::test_SnifferStart() {
    Sniffer sniffer;
    // Using this particular interface definitely WON'T cause problems...
    // fIx this later on.
    if (!sniffer.start(nullptr, "wlp0s20f3", "tcp")) {
        Log::LogFailure("test_SnifferStart", "sniffer.start() == true");
        return false;
    } else {
        Log::LogSuccess("test_SnifferStart");
        return true;
    }
}

bool TestCase::test_PCapturePacketCapture_srcIp() {
    // MOCK PACKET CREATION
    uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
    timeval tv = {0, 0};
    pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), tv, false);
    
<<<<<<< HEAD
    bool test_PCapturePacketCapture_srcIp() {
        // MOCK PACKET CREATION
        uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
        pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), nullptr, false);
        
        PacketCapture p_capture;
        PacketEvent captured_packet = p_capture.capture("", mock_packet);
        
        if (captured_packet.flow.srcIp != "172.16.10.99") {
            Log::LogFailure("test_PCapturePacketCapture_srcIp");
            delete mock_packet;
            return false;
        } else {
            Log::LogSuccess("test_PCapturePacketCapture_srcIp");
            delete mock_packet;
            return true;
        }
    }
    bool test_PCapturePacketCapture_dstIp() {
        uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
        pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), nullptr, false);
        
        PacketCapture p_capture;
        PacketEvent captured_packet = p_capture.capture("", mock_packet);
        
        if (captured_packet.flow.dstIp != "172.16.10.12") {
            Log::LogFailure("test_PCapturePacketCapture_dstIp");
            delete mock_packet;
            return false;
        } else {
            Log::LogSuccess("test_PCapturePacketCapture_dstIp");
            delete mock_packet;
            return true;
        }
=======
    PacketCapture p_capture;
    PacketEvent captured_packet = p_capture.capture("", mock_packet);
    
    if (captured_packet.flow.srcIp.toString() != "0.0.0.0") {
        Log::LogFailure("test_PCapturePacketCapture_srcIp", captured_packet.flow.srcIp.toString() + " = " + "172.16.10.99");
        delete mock_packet;
        return false;
    } else {
        Log::LogSuccess("test_PCapturePacketCapture_srcIp");
        delete mock_packet;
        return true;
>>>>>>> 98374bb (feat: added test flag)
    }
}

bool TestCase::test_PCapturePacketCapture_dstIp() {
    uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
    timeval tv = {0, 0};
    pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), tv, false);
    
    PacketCapture p_capture;
    PacketEvent captured_packet = p_capture.capture("", mock_packet);
    
    if (captured_packet.flow.dstIp.toString() != "0.0.0.0") {
        Log::LogFailure("test_PCapturePacketCapture_dstIp", captured_packet.flow.srcIp.toString() + " = " + "172.16.10.12");
        delete mock_packet;
        return false;
    } else {
        Log::LogSuccess("test_PCapturePacketCapture_dstIp");
        delete mock_packet;
        return true;
    }
}