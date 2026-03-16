#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <optional>
#include <arpa/inet.h>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"
#include "pcapplusplus/include/Packet.h"

#include "tests/tests.h"
#include "args/args.hxx"

#include "PacketEvent.h"
#include "Sniffer.h"
#include "PacketCapture.h"
#include "PacketFilter.h"
#include "AhoCorasickDFA.h"
#include "AppOptions.h"
#include "Sink.h"

bool ArgParse(const int& argc, char** argv) {
    args::ArgumentParser parser(
        "RegSpy",
        "A finite-automata, pattern-based IDS solution for containerised environments."
    );
    
    args::Flag verbose(
        parser,
        "verbose",
        "Log all the progress of the app.",
        {'v', "verbose"}
    );
    
    args::Flag test(
        parser,
        "test",
        "Run unit tests rather than the app itself.",
        {'t', "test"}
    );

    args::HelpFlag help(
        parser,
        "help",
        "Display this help menu.",
        {'h', "help"}
    );
    
    args::ValueFlag<std::string> device(
        parser,
        "device",
        "Specify the network interface to listen on.",
        {'d', "device"}
    );
    
    args::CompletionFlag completion(parser, {"complete"}); // Tells the argument parser to stop looking for arguments.

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion& e) {
        std::cout << e.what();
        return false;
    } catch (const args::Help&) {
        std::cout << parser;
        return false;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return true;
    }
    
    g_app_options.verbose = verbose.Get();
    g_app_options.test = test.Get();
    g_app_options.device = device.Get();

    return true;
}

/*
This is a very simple implementation. The next step is to implement a few more things. We need to do the following:
- Capture - Capture the packets with the eBPF filter and network devices.
- Decode - Turn the raw packets into structured events that we can properly analyse
- Detect - Detect the packets with pluggable extendable detectors
- Output - Pluggable functions like output to JSON, SQLite, or Message queues.
- Config - Self explanatory.

*/
void onPacketArrive(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie) {
    std::cout << "huh???" << std::endl;
    if (!packet || !device) {
        return;
    }

    PacketFilter p_filter;
    p_filter.dstIp = pcpp::IPv4Address("10.58.119.69");

    PacketCapture p_capture;
    if (g_app_options.verbose) {
        std::cout << "Listening!" << std::endl;
    }

    //Sink sink = Sink(p_filter, p_capture);
    
    //sink.Run(packet, device, cookie);
}

int main(int argc, char** argv) {
    if (!ArgParse(argc, argv)) {
        std::cout << "error: Could not setup command-line arguments. Exiting..." << std::endl;
        return 1;
    }

    if (g_app_options.verbose) {
        std::cout << "info: Verbose flag detected" << std::endl;
    }
    
    if (g_app_options.test) {
        TestCase::test_PCapturePacketCapture_dstIp();
        TestCase::test_PCapturePacketCapture_srcIp();
        TestCase::test_SnifferStart();
        TestCase::test_AhoCorasickNode_functions();
        TestCase::test_AhoCorasickDFA();
        return 0;
    }

    Sniffer sniffer;
    if (!sniffer.start(onPacketArrive, g_app_options.device, "tcp")) {
        std::cout << "error: Failed to start the interface with device: " << g_app_options.device << std::endl;
        return 0;
    }

    if (g_app_options.verbose) {
        std::cout << "info: started listening on device: " << g_app_options.device << std::endl;
    }
    while (true) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }

    sniffer.stop();
}