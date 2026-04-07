#pragma once

#include <string>

struct Rule {
    std::string id;
    std::string pattern;
    std::string severity;  // "low" | "medium" | "high" | "critical"
};
