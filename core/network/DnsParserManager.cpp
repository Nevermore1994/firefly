//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "DnsParserManager.hpp"
#include "ThreadManager.hpp"
#include "NetUtility.hpp"
#include <future>

using namespace firefly;
using namespace firefly::Network;

DnsParserRequest::DnsParserRequest(DnsHostInfo&& i, DnsParserCallBack c)
    :info(i)
    ,callBack(std::move(c)){
    
}

DnsParserManager::~DnsParserManager() {
    ThreadManager::shareInstance().remove(work_);
    std::unique_lock<std::mutex> lock(mutex_);
    requests_.clear();
    ipLists_.clear();
    work_->stop();
}

DnsParserManager::DnsParserManager()
    :work_(std::make_shared<Thread>("DnsParserManager",  &DnsParserManager::process, this)){
    ThreadManager::shareInstance().add(work_);
}

void DnsParserManager::addRequest(DnsParserRequest&& info) noexcept {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        requests_.push_back(std::move(info));
    }
    if(!work_->isRunning()){
        work_->resume();
    }
}

void DnsParserManager::process() noexcept {
    decltype(requests_) requests;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        requests.swap(requests_);
    }
    if(requests.empty()){
        work_->pause();
        return;
    }
    for(auto& request:requests){
        Network::IPAddressInfo ipAddressInfo;
        if(Network::parseHost(request.info.host, ipAddressInfo)){
            request.callBack(request.info, ipAddressInfo);
        }
    }
}

bool DnsParserManager::parseHost(const std::string& host, IPAddressInfo& ip) noexcept {
    auto res = std::async(std::launch::async, [&host, &ip](){
       return Network::parseHost(host, ip);
    });
    res.wait();
    return res.get();
}

bool DnsParserManager::parseHost(std::string&& host, IPAddressInfo& ip) noexcept {
    return parseHost(host, ip);
}

void DnsParserManager::parseHost(DnsParserRequest&& info) noexcept {
    addRequest(std::move(info));
}

std::string DnsParserManager::getMyHost() noexcept {
    if(!isInitialize_){
        init();
    }
    return myHost_;
}

IPAddressInfo DnsParserManager::getMyIP() noexcept {
    if(!isInitialize_){
        init();
    }
    return myIP_;
}

void DnsParserManager::init() noexcept {
    std::call_once(flag_, [this](){
        auto res = std::async(std::launch::async, [this](){
            bool isSuccess = false;
            this->myHost_ = Network::getHostName();
            isSuccess = Network::getLocalAddress(this->myIP_);
            return isSuccess;
        });
        res.get();
        this->isInitialize_ = true;
    });
    
}


