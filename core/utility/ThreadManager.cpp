//
// Created by Nevermore on 2021/10/25.
// firefly ThreadManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "ThreadManager.hpp"
#include "Log.hpp"
#include "TimerManager.h"

using namespace firefly;
using namespace firefly::Util;

ThreadManager::ThreadManager() {
    TimerManager::shareInstance().runLoop(10 * 1000, [this](){
        reportRunInfo();
    });
}

ThreadManager::~ThreadManager() {
    std::unique_lock<std::mutex> lock(mutex_);
    threadInfos_.clear();
}

void ThreadManager::add(std::shared_ptr<Thread> thread) {
    std::unique_lock<std::mutex> lock(mutex_);
    threadInfos_.emplace(std::make_pair(thread->getId(), std::move(thread)));
}

void ThreadManager::remove(std::shared_ptr<Thread> thread) {
    std::unique_lock<std::mutex> lock(mutex_);
    threadInfos_.erase(thread->getId());
}

void ThreadManager::remove(std::thread::id id) {
    std::unique_lock<std::mutex> lock(mutex_);
    threadInfos_.erase(id);
}

Thread& ThreadManager::thisThread() {
    std::unique_lock<std::mutex> lock(mutex_);
    auto id = std::this_thread::get_id();
    if(threadInfos_.count(id)){
        return *threadInfos_[id];
    } else {
        throw std::runtime_error("this thread not belong to ThreadManager.");
    }
}

void ThreadManager::reportRunInfo() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    TimeStamp now = nowTimeStamp();
    logi("ThreadManager report now:%llu, thread:%lu", now, threadInfos_.size());
    for(auto& pair:threadInfos_){
        auto thread = pair.second;
        TimeStamp interval = (now - thread->getLastRunTimeStamp());
        if(thread->isRunning() && interval >= kMaxTimeInterval){
            loge("ThreadManager report [%s] is blocking.", thread->getName().c_str());
        }
    }
}
