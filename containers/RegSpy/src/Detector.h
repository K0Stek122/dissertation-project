#pragma once

#include <vector>
#include <map>
#include <string>

#include "Rule.h"
#include "Alert.h"
#include "PacketEvent.h"
#include "AhoCorasickDFA.h"

class Detector {
private:
    AhoCorasick ac;
    std::map<std::string, Rule> pattern_to_rule;

public:
    explicit Detector(const std::vector<Rule>& rules);
    std::vector<Alert> detect(const PacketEvent& event) const;
};
