#include <iostream>
#include <vector>

#include "pcapplusplus/include/PcapLiveDeviceList.h"
#include "pcapplusplus/include/PcapLiveDevice.h"
#include "pcapplusplus/include/SystemUtils.h"

int main(int, char**) {
    auto devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    std::vector<pcpp::PcapLiveDevice*>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        std::cout << *iter << std::endl;        
    }
}