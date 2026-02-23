#include "IDS.h"

bool IDS::match(const PacketEvent &p, const Signature &signature)
{
    if (p.data.len < signature.pattern.size()) {
        return false;
    }
    
    for (size_t i = 0; i <= p.data.len - signature.pattern.size(); i++) {
        if (std::equal(signature.pattern.begin(), signature.pattern.end(), p.data.payload.begin() + i)) {
            return true;
        }
    }
    return false;
}