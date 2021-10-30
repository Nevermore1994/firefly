//
//  test.h
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//
#pragma once

#include "testThread.hpp"
#include "testPacketPool.h"
#include "testTimer.hpp"
#include "testFile.hpp"
#include "NetUtility.hpp"
#include "DnsParserManager.hpp"

using namespace firefly;
using namespace firefly::Network;

inline void runTest(){
    //testPackPool();
    //testThread();
    //testTimer();
    //testFile();
    IPAddressInfo ip;
    parseHost("baidu.com", ip);
    std::cout << ip2str(ip) << std::endl;
    
    std::cout << ip2str(DnsParserManager::shareInstance().getMyIP()) << std::endl;
}
