//
// Created by Nevermore on 2021/10/25.
// firefly ThreadManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <thread>
#include <unordered_map>
#include "Thread.hpp"

namespace firefly{

constexpr const Util::TimeStamp kMaxTimeInterval = 30 * 1000 * 1000;//30s

class ThreadManager{
public:
    static inline ThreadManager& shareInstance(){
        static ThreadManager instance;
        std::this_thread::get_id();
        return instance;
    }

public:
    ThreadManager();
    ~ThreadManager();
    ThreadManager(const ThreadManager&) = delete;
    ThreadManager& operator=(const ThreadManager&) = delete;
    ThreadManager(ThreadManager&&) = delete;
    ThreadManager& operator=(ThreadManager&&) = delete;
    
    void add(std::shared_ptr<Thread> thread);
    void remove(const std::shared_ptr<Thread>& thread);
    void remove(std::thread::id id);
    
    Thread& thisThread();
private:
    void reportRunInfo() noexcept;
private:
    std::unordered_map<std::thread::id, std::shared_ptr<Thread>> threadInfos_;
    std::mutex mutex_;
};

}

