#include "SyslogOutput.h"

#include <syslog.h>

static int severity_to_priority(const std::string& severity) {
    if (severity == "critical") return LOG_CRIT;
    if (severity == "high")     return LOG_ERR;
    if (severity == "medium")   return LOG_WARNING;
    return LOG_INFO;
}

SyslogOutput::SyslogOutput() {
    openlog("RegSpy", LOG_PID | LOG_NDELAY, LOG_DAEMON);
}

SyslogOutput::~SyslogOutput() {
    closelog();
}

void SyslogOutput::write(const Alert& alert) {
    syslog(
        severity_to_priority(alert.severity),
        "[RegSpy] rule=%s src=%s:%d dst=%s:%d pattern=%s",
        alert.rule_id.c_str(),
        alert.flow.srcIp.toString().c_str(),
        alert.flow.srcPort,
        alert.flow.dstIp.toString().c_str(),
        alert.flow.dstPort,
        alert.matched_pattern.c_str()
    );
}
