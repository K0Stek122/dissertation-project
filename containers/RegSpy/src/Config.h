#pragma once

#include <string>
#include <vector>

#include "Rule.h"

struct AppConfig {
    std::string device;
    std::string rules_file;
    std::string sqlite_path = "/var/log/regspy.db";
    bool syslog_enabled = true;
    bool verbose = false;
    std::vector<Rule> rules;
};

AppConfig load_config(const std::string& path);
