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
#include <future>
#include "Thread.hpp"
#include "NetworkType.hpp"

namespace firefly::Network{

struct DnsHostInfo{
    std::string uuid;
    std::string host;
    
    DnsHostInfo() = default;
    explicit DnsHostInfo(std::string&& h)
        :host(std::forward<std::string>(h)){
        
    }
};

using DnsParserCallBack = std::function<void(DnsHostInfo,IPAddressInfo)>;
struct DnsParserRequest{
    DnsHostInfo info;
    DnsParserCallBack callBack;
    
    DnsParserRequest() = default;
    ~DnsParserRequest() = default;
    DnsParserRequest(DnsHostInfo&& info, DnsParserCallBack callBack);
};

class DnsParserManager {

public:
    static inline DnsParserManager& shareInstance() {
        static DnsParserManager instance;
        return instance;
    }
    
public:
    ~DnsParserManager() = default;
    
    DnsParserManager(const DnsParserManager&) = delete;
    
    DnsParserManager& operator=(const DnsParserManager&) = delete;
    
    DnsParserManager(DnsParserManager&&) = delete;
    
    DnsParserManager& operator=(DnsParserManager&&) = delete;
    
    static bool parseHost(const std::string& host, IPAddressInfo& ip) noexcept;
    
    static bool parseHost(std::string&& host, IPAddressInfo& ip) noexcept;
    
    void parseHost(DnsParserRequest&& info) noexcept;
    
    std::string getMyHost() noexcept;
    
    IPAddressInfo getMyIP() noexcept;
    
    void release() noexcept;

private:
    DnsParserManager();
    
    void process() noexcept;
    
    void addRequest(DnsParserRequest&& info) noexcept;
    
    void init() noexcept;

private:
    std::unordered_multimap<std::string, IPAddressInfo> ipLists_;
    std::deque<DnsParserRequest> requests_;
    std::mutex mutex_;
    std::shared_ptr<Thread> work_;
    std::string myHost_;
    IPAddressInfo myIP_;
    bool isInitialize_ = false;
    std::once_flag flag_;
};

}



