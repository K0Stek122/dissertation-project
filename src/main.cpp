#include <iostream>
#include "check_dependencies.h"

int main(int, char**){

    std::string distro = get_linux_distribution();
    // TODO: Support more distributions.
    if (distro != "fedora") {
        std::cout << "Error: Unsupported distribution: " << distro << ". Please use one of supported distributions: fedora" << std::endl;
        return 1;
    }
    

}