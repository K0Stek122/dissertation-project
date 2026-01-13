#include <iostream>
#include <vector>

#include "include/pcapplusplus/PcapLiveDeviceList.h"
#include "include/pcapplusplus/PcapLiveDevice.h"
#include "include/pcapplusplus/SystemUtils.h"

int main(int, char**) {
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    std::vector<pcpp::PcapLiveDevice*>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        std::cout << *iter << std::endl;        
    }
}