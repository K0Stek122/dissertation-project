#include "Config.h"

#include <yaml-cpp/yaml.h>
#include <stdexcept>

AppConfig load_config(const std::string& path) {
    AppConfig cfg;

    YAML::Node doc;
    try {
        doc = YAML::LoadFile(path);
    } catch (const YAML::Exception& e) {
        throw std::runtime_error("Failed to load config file '" + path + "': " + e.what());
    }

    if (doc["device"])      cfg.device       = doc["device"].as<std::string>();
    if (doc["sqlite_path"]) cfg.sqlite_path  = doc["sqlite_path"].as<std::string>();
    if (doc["syslog"])      cfg.syslog_enabled = doc["syslog"].as<bool>();
    if (doc["verbose"])     cfg.verbose      = doc["verbose"].as<bool>();

    if (doc["rules"]) {
        for (const auto& r : doc["rules"]) {
            if (!r["id"] || !r["pattern"] || !r["severity"]) {
                throw std::runtime_error("Each rule must have 'id', 'pattern', and 'severity' fields.");
            }
            Rule rule;
            rule.id       = r["id"].as<std::string>();
            rule.pattern  = r["pattern"].as<std::string>();
            rule.severity = r["severity"].as<std::string>();
            cfg.rules.push_back(rule);
        }
    }

    return cfg;
}
