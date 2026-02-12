#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"
#include "pcapplusplus/include/Packet.h"

#include "args/args.hxx"

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
static void onPacketArrive(pcpp::RawPacket* rawPacket, pcpp::PcapLiveDevice* dev, void*) {
    pcpp::Packet packet(rawPacket);
    std::cout << "Packet captured: " << packet << std::endl;
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

    // Get all devices. The first one will be the closest hit.
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    if (options.verbose) {
        std::cout << "Detected network devices: " << std::endl;
        for (auto iter = devices.begin(); iter != devices.end(); iter++) {
            std::cout << (*iter)->getName() << std::endl;
        }
    }
    
    auto* pcap_dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(devices[0]->getName());
    
    if (!pcap_dev) {
        std::cerr << "Cannot find any network devices." << std::endl;
        return 1;
    }

    // Opens the socket via pcap
    if (!pcap_dev->open()) {
        std::cerr << "Cannot open the pcap device." << std::endl;
        return 1;
    }
    
    pcap_dev->setFilter("tcp");
    pcap_dev->startCapture(onPacketArrive, nullptr);
    
    std::this_thread::sleep_for(std::chrono::seconds(10));

    pcap_dev->stopCapture();
    pcap_dev->close();
    
}