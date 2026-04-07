#include "Detector.h"

Detector::Detector(const std::vector<Rule>& rules)
    : ac([&]{
        std::vector<std::string> patterns;
        patterns.reserve(rules.size());
        for (const auto& r : rules) {
            patterns.push_back(r.pattern);
        }
        return patterns;
    }())
{
    for (const auto& r : rules) {
        pattern_to_rule[r.pattern] = r;
    }
}

std::vector<Alert> Detector::detect(const PacketEvent& event) const {
    std::vector<Alert> alerts;

    if (event.payload.empty()) {
        return alerts;
    }

    const std::string payload_str(event.payload.begin(), event.payload.end());
    const auto matches = ac.search(payload_str);

    for (const auto& match : matches) {
        auto it = pattern_to_rule.find(match.val);
        if (it == pattern_to_rule.end()) {
            continue;
        }
        const Rule& rule = it->second;

        Alert alert;
        alert.rule_id         = rule.id;
        alert.severity        = rule.severity;
        alert.matched_pattern = match.val;
        alert.match_offset    = match.pos;
        alert.flow            = event.flow;
        alert.tcp_flags       = event.tcpFlags;
        alert.timestamp       = event.timestamp;
        alerts.push_back(alert);
    }

    return alerts;
}
