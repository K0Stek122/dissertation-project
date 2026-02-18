#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <optional>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"
#include "pcapplusplus/include/Packet.h"

#include "args/args.hxx"

#include "PacketEvent.h"
#include "Sniffer.h"
#include "PacketCapture.h"
#include "PacketFilter.h"

struct AppOptions {
    bool verbose = false;
};

bool setup_arguments(int argc, char** argv, AppOptions& options) {
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

    args::HelpFlag help(
        parser,
        "help",
        "Display this help menu.",
        {'h', "help"}
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
    
    options.verbose = verbose;

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
    if (!packet || !device) {
        return;
    }

    PacketFilter p_filter;
    p_filter.dstIp = pcpp::IPv4Address("10.58.119.69");

    PacketCapture p_capture;
    p_capture.add_filter(p_filter);

    //p_capture.add_filter(p_filter);
    std::optional<PacketEvent> captured_packet = p_capture.capture(packet);
    
    if (captured_packet.has_value()) {
        std::cout << captured_packet.value().flow.dstIp << std::endl;
    }
}

int main(int argc, char** argv) {
    
    AppOptions options;

    if (!setup_arguments(argc, argv, options)) {
        std::cout << "error: Could not setup command-line arguments. Exiting..." << std::endl;
        return 1;
    }

    if (options.verbose) {
        std::cout << "info: Verbose flag detected" << std::endl;
    }

    Sniffer sniffer;
    sniffer.start(onPacketArrive, "wlp0s20f3", "tcp");
    while (true) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
    sniffer.stop();
}