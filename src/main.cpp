#include <iostream>
#include <vector>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"

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

int main(int argc, char** argv) {
    
    AppOptions options;

    if (!setup_arguments(argc, argv, options)) {
        return 1;
    }

    if (options.verbose) {
        std::cout << "Verbose flag detected" << std::endl;
    }

    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    if (options.verbose) {
        std::cout << "Detected network devices: " << std::endl;
        for (auto iter = devices.begin(); iter != devices.end(); iter++) {
            std::cout << (*iter)->getName() << std::endl;
        }
    }
}