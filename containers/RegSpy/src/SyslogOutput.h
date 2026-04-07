#pragma once

#include "Alert.h"

class SyslogOutput {
public:
    SyslogOutput();
    ~SyslogOutput();

    SyslogOutput(const SyslogOutput&) = delete;
    SyslogOutput& operator=(const SyslogOutput&) = delete;

    void write(const Alert& alert);
};
