#include <sys/utsname.h> // Library that lets us check the currently ran linux distribution.
#include <string>

 check_linux_distribution() {
     utsname* buf = nullptr;
    int ret_code = uname(utsname);
}