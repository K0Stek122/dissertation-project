#pragma once

#include <optional>

#include "Alert.h"
#include "Config.h"
#include "SqliteOutput.h"
#include "SyslogOutput.h"

class OutputManager {
private:
    std::optional<SqliteOutput> sqlite;
    std::optional<SyslogOutput> syslog_out;

public:
    explicit OutputManager(const AppConfig& cfg);

    void emit(const Alert& alert);
};
