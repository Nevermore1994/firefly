//
// Created by Nevermore on 2021/10/22.
// firefly test/test
// Copyright (c) 2021 Nevermore All rights reserved.
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
