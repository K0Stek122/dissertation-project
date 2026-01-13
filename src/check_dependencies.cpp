#include <sys/utsname.h> // Library that lets us check the currently ran linux distribution.
#include <string>

#include "check_dependencies.h"

std::string get_linux_distribution() {
    utsname buf;
    int success = uname(&buf);
    if (success != 0) {
        return std::string("Grave error! Could not receive uname.");
    }
    std::string ret = buf.nodename;
    return ret;
}