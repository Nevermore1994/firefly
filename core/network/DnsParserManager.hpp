//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <unordered_map>
#include <deque>
#include <mutex>
#include <memory>
#include "Thread.hpp"
#include "IPAddress.hpp"

namespace firefly::Network{

struct DnsParserInfo{
    std::string uuid;
    std::string host;
    
};

class DnsParserManager {

public:
    static inline DnsParserManager& shareInstance() {
        static DnsParserManager instance;
        return instance;
    }
    
public:
    ~DnsParserManager();
    DnsParserManager(const DnsParserManager&) = delete;
    DnsParserManager& operator=(const DnsParserManager&) = delete;
    DnsParserManager(DnsParserManager&&) = delete;
    DnsParserManager& operator=(DnsParserManager&&) = delete;
    
    void addRequest(DnsParserInfo&& info) noexcept;
private:
    DnsParserManager();
    
    void process() noexcept;
private:
    std::unordered_multimap<std::string, IPAddressInfo> ipLists_;
    std::deque<DnsParserInfo> requests_;
    std::mutex mutex_;
    std::unique_ptr<Thread> work_;
};

}



