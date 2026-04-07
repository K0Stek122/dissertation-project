#include "OutputManager.h"

#include <iostream>

OutputManager::OutputManager(const AppConfig& cfg) {
    if (!cfg.sqlite_path.empty()) {
        sqlite.emplace(cfg.sqlite_path);
    }
    if (cfg.syslog_enabled) {
        syslog_out.emplace();
    }
}

void OutputManager::emit(const Alert& alert) {
    if (sqlite.has_value()) {
        sqlite->write(alert);
    }
    if (syslog_out.has_value()) {
        syslog_out->write(alert);
    }
}
