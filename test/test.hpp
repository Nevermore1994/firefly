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
#include "ThreadManager.hpp"
#include "ConnectorManager.hpp"
#include "NetEngine.hpp"

using namespace firefly;
using namespace firefly::Network;

inline void runTest() {
    //testPackPool();
    //testThread();
    //testTimer();
    //testFile();
    IPAddressInfo ip;
    parseHost("baidu.com", ip);
    std::cout << ip2str(ip) << std::endl;
    
    using namespace std::chrono_literals;
    DnsHostInfo info("qq.com");
    DnsParserRequest request(std::move(info), [](const DnsHostInfo& i, IPAddressInfo ip) {
        std::cout << i.host << ":" << ip2str(ip) << std::endl;
    });
    DnsParserManager::shareInstance().parseHost(std::move(request));
    std::cout << "self:" << ip2str(DnsParserManager::shareInstance().getMyIP()) << std::endl;
    std::cout << "---- test end --------" << std::endl;
    std::this_thread::sleep_for(50ms);
}
