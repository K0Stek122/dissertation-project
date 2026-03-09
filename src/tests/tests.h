#ifndef TESTS_H
#define TESTS_H

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
#include "../AhoCorasickDFA.h"

namespace Log {
    void LogFailure(const std::string& information, const std::string& assert_info);
    void LogSuccess(const std::string& information);
}

namespace TestCase {
    bool test_SnifferStart();
    bool test_PCapturePacketCapture_srcIp();
    bool test_PCapturePacketCapture_dstIp();
    
    bool test_AhoCorasickNode_functions();
}

#endif // TESTS_H