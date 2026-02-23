#pragma once
#include <vector>
#include <cstdint>

#include "PacketEvent.h"

struct Signature {
    std::vector<uint8_t> pattern;
};

class IDS {
// VARIABLES
private:
public:

// METHODS
private:
public:
    bool match(const PacketEvent& p, const Signature& signature);
};