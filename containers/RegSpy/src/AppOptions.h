#pragma once

#include <string>

struct AppOptions {
    bool verbose = false;
    bool test = true;
    std::string device;
    std::string config_file;
} g_app_options;