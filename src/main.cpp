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
#include "IDS.h"

struct AppOptions {
    bool verbose = false;
} appOptions;

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


void onPacketArrive(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie) {
    if (!packet || !device) {
        return;
    }

    PacketFilter p_filter;
    p_filter.dstPort = 443;
    PacketFilter p_filter2;
    p_filter2.dstPort = 52484;

    PacketCapture p_capture;
    p_capture.add_filter(p_filter);
    p_capture.add_filter(p_filter2);

    //p_capture.add_filter(p_filter);
    std::optional<PacketEvent> captured_packet = p_capture.capture(packet);
    
    IDS ids_system;
    Signature test_signature;
    test_signature.pattern = {246,53,222,78,79,159,218,171,82,13,192,74,8};

    if (captured_packet.has_value()) {
        if (appOptions.verbose) {
            std::cout << "PACKET CAPTURED: "
                        << std::endl
                        << "dstIp: " << captured_packet.value().flow.dstIp << std::endl
                        << "srcIp: " << captured_packet.value().flow.srcIp << std::endl
                        << "dstPort: " << captured_packet.value().flow.dstPort << std::endl
                        << "srcPort: " << captured_packet.value().flow.srcPort << std::endl << std::endl;
        }
    }
    
    
}

int main(int argc, char** argv) {
    if (!setup_arguments(argc, argv, appOptions)) {
        std::cout << "error: Could not setup command-line arguments. Exiting..." << std::endl;
        return 1;
    }

    if (appOptions.verbose) std::cout << "info: Verbose flag detected" << std::endl;

    if (appOptions.verbose) std::cout << "Starting packet sniffing..." << std::endl;

    Sniffer sniffer;
    sniffer.start(onPacketArrive, "wlp0s20f3", "tcp");

    if (appOptions.verbose) std::cout << "Packet Sniffing started on wlp0s20f3" << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
    sniffer.stop();
}