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
    if (!sniffer.start(nullptr, "", "tcp")) {
        Log::LogFailure("test_SnifferStart", "sniffer.start() == true");
        sniffer.stop();
        return false;
    } else {
        Log::LogSuccess("test_SnifferStart");
        sniffer.stop();
        return true;
    }
}

bool TestCase::test_PCapturePacketCapture_srcIp() {
    uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
    timeval tv = {0, 0};
    pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), tv, false);
    PacketCapture p_capture;
    std::optional<PacketEvent> captured_packet = p_capture.capture(mock_packet);
    std::string srcIpStr = captured_packet.value().flow.srcIp.toString();
    if (srcIpStr != "0.0.0.0") {
        Log::LogFailure("test_PCapturePacketCapture_srcIp", srcIpStr + " != 0.0.0.0");
        delete mock_packet;
        return false;
    } else {
        Log::LogSuccess("test_PCapturePacketCapture_srcIp");
        delete mock_packet;
        return true;
    }
}

bool TestCase::test_PCapturePacketCapture_dstIp() {
    uint8_t packet_data[] = {0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xac, 0x10, 0x0a, 0x63, 0xac, 0x10, 0x0a, 0x0c};
    timeval tv = {0, 0};
    pcpp::RawPacket* mock_packet = new pcpp::RawPacket(packet_data, sizeof(packet_data), tv, false);
    PacketCapture p_capture;
    std::optional<PacketEvent> captured_packet = p_capture.capture(mock_packet);
    std::string dstIpStr = captured_packet.value().flow.dstIp.toString();
    if (dstIpStr != "0.0.0.0") {
        Log::LogFailure("test_PCapturePacketCapture_dstIp", dstIpStr + " != 0.0.0.0");
        delete mock_packet;
        return false;
    } else {
        Log::LogSuccess("test_PCapturePacketCapture_dstIp");
        delete mock_packet;
        return true;
    }
}

bool TestCase::test_AhoCorasickNode_functions() {
    Node node;

    // Test hasChild on empty node
    if (node.hasChild('a')) {
        Log::LogFailure("test_AhoCorasickNode_functions", "hasChild('a') == false on empty node");
        return false;
    }

    // Test setChild and getChild — node takes ownership via unique_ptr
    Node* child_a_ptr = new Node();
    node.setChild('a', std::unique_ptr<Node>(child_a_ptr));
    if (!node.hasChild('a')) {
        Log::LogFailure("test_AhoCorasickNode_functions", "hasChild('a') == true after setChild");
        return false;
    }

    if (node.getChild('a') != child_a_ptr) {
        Log::LogFailure("test_AhoCorasickNode_functions", "getChild('a') returns correct child");
        return false;
    }

    // Test setChild with multiple children
    node.setChild('b', std::make_unique<Node>());
    if (!node.hasChild('b')) {
        Log::LogFailure("test_AhoCorasickNode_functions", "hasChild('b') == true after setChild");
        return false;
    }

    // Test addOutput
    node.addOutput("test_output");
    if (node.outputs.empty()) {
        Log::LogFailure("test_AhoCorasickNode_functions", "addOutput adds output to set");
        return false;
    }

    // Test copyOutputs
    Node target_node;
    target_node.copyOutputs(node);
    if (target_node.outputs.size() != node.outputs.size()) {
        Log::LogFailure("test_AhoCorasickNode_functions", "copyOutputs copies all outputs");
        return false;
    }

    // Test failureLink initialization
    if (node.failureLink != nullptr) {
        Log::LogFailure("test_AhoCorasickNode_functions", "failureLink initialized to nullptr");
        return false;
    }

    Log::LogSuccess("test_AhoCorasickNode_functions");
    return true;
}

bool TestCase::test_AhoCorasickDFA()
{
    std::vector<std::string> text_to_search_for = {"eat", "eating", "meat", "in"};
    std::string text = "i am eating meat";
    auto machine = AhoCorasick(text_to_search_for);
    auto result = machine.search(text);

    std::vector<FoundString> expected_result = {
        {5, "eat"},
        {8, "in"},
        {5, "eating"},
        {13, "eat"},
        {12, "meat"}
    };

    if (expected_result != result) {
        Log::LogFailure("test_AhoCorasickDFA", "expected_result != result");
        return false;
    }

    Log::LogSuccess("test_AhoCorasickDFA");
    return true;
}
