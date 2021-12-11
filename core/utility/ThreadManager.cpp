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
    timerId_ = TimerManager::shareInstance().runLoop(10 * 1000, [this]() {
        reportRunInfo();
    });
}

ThreadManager::~ThreadManager() {
    release();
}

void ThreadManager::add(std::shared_ptr<Thread> thread) {
    std::unique_lock<std::mutex> lock(mutex_);
    threadInfos_.emplace(std::make_pair(thread->getId(), std::move(thread)));
}

void ThreadManager::remove(const std::shared_ptr<Thread>& thread) {
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
        auto thread = threadInfos_[id];
        if(thread.expired()){
            throw std::runtime_error("this thread is expired.");
        }
        return *thread.lock();
    } else {
        throw std::runtime_error("this thread not belong to ThreadManager.");
    }
}

void ThreadManager::reportRunInfo() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    TimeStamp now = nowTimeStamp();
    logi("ThreadManager report now:%llu, thread:%lu", now, threadInfos_.size());
    //todo replace C++20 std::erase_if
    std::vector<std::thread::id> expiredThreads;
    for(auto& pair:threadInfos_){
        auto t = pair.second;
        if(t.expired()){
            expiredThreads.push_back(pair.first);
            continue;
        }
        auto thread = t.lock();
        TimeStamp interval = (now - thread->getLastRunTimeStamp());
        if (thread->isRunning() && interval >= kMaxThreadBlockTimeInterval) {
            loge("ThreadManager report [%s] is blocking.", thread->getName().data());
        }
    }
    for (auto key: expiredThreads) {
        threadInfos_.erase(key);
    }
}

void ThreadManager::release() noexcept {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        threadInfos_.clear();
    }
    TimerManager::shareInstance().cancel(timerId_);
}
