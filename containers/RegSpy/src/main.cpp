#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <csignal>

#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/RawPacket.h"

#include "tests/tests.h"
#include "args/args.hxx"

#include "AppOptions.h"
#include "Config.h"
#include "PacketEvent.h"
#include "Sniffer.h"
#include "Sink.h"
#include "Detector.h"
#include "OutputManager.h"

static std::atomic<bool> g_running{true};

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

    args::ValueFlag<std::string> config(
        parser,
        "config",
        "Path to the YAML config file.",
        {'c', "config"}
    );

    args::CompletionFlag completion(parser, {"complete"});

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

    g_app_options.verbose     = verbose.Get();
    g_app_options.test        = test.Get();
    g_app_options.device      = device.Get();
    g_app_options.config_file = config.Get();

    return true;
}

void onPacketArrive(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* device, void* cookie) {
    if (!packet || !device || cookie == nullptr) {
        return;
    }
    Sink* sink = static_cast<Sink*>(cookie);
    sink->Run(packet, device, cookie);
}

int main(int argc, char** argv) {
    if (!ArgParse(argc, argv)) {
        return 1;
    }

    if (g_app_options.test) {
        TestCase::test_PCapturePacketCapture_dstIp();
        TestCase::test_PCapturePacketCapture_srcIp();
        TestCase::test_SnifferStart();
        TestCase::test_AhoCorasickNode_functions();
        TestCase::test_AhoCorasickDFA();
        return 0;
    }

    // Load config file, then apply CLI overrides on top
    AppConfig cfg;
    if (!g_app_options.config_file.empty()) {
        try {
            cfg = load_config(g_app_options.config_file);
        } catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << std::endl;
            return 1;
        }
    }

    if (!g_app_options.device.empty()) cfg.device  = g_app_options.device;
    if (g_app_options.verbose)         cfg.verbose = true;

    if (cfg.verbose) {
        std::cout << "info: loaded " << cfg.rules.size() << " rule(s)" << std::endl;
    }

    Detector detector(cfg.rules);
    OutputManager output(cfg);
    Sink sink(detector, output);

    signal(SIGINT,  [](int){ g_running = false; });
    signal(SIGTERM, [](int){ g_running = false; });

    Sniffer sniffer;
    if (!sniffer.start(onPacketArrive, cfg.device, "tcp", &sink)) {
        std::cerr << "error: Failed to start capture on device: "
                  << (cfg.device.empty() ? "(default)" : cfg.device) << std::endl;
        return 1;
    }

    if (cfg.verbose) {
        std::cout << "info: listening on device: "
                  << (cfg.device.empty() ? "(default)" : cfg.device) << std::endl;
    }

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    sniffer.stop();
    return 0;
}
