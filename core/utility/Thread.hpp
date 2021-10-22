//
// Created by Nevermore on 2021/10/22.
// firefly Thread
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <functional>
#include <atomic>
#include "Utility.hpp"

namespace firefly {

class Thread{

public:
    template<class Func, typename ... Args>
    Thread(std::string&& name, Func&& f, Args&& ... args)
        :name_(std::move(name))
        ,isRunning_(false)
        ,isExit_(false)
        ,worker_(&Thread::process, this)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
    
    template<class Func, typename ... Args>
    Thread(const std::string& name, Func&& f, Args&& ... args)
        :name_(name)
        ,isRunning_(false)
        ,isExit_(false)
        ,worker_(&Thread::process, this)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
    
    explicit Thread(std::string&& name);
    
    explicit Thread(const std::string& name);
    
    ~Thread() noexcept;
    
    void start() noexcept;
    
    void pause() noexcept;
    
    void resume() noexcept;
    
    void stop() noexcept;
    
    template<class Func, typename ... Args>
    void setFunc(Func&& f, Args&& ... args) noexcept{
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
public:
    inline bool isRunning() const noexcept {
        return isRunning_.load();
    }
private:
    void process() noexcept;
    void setThreadName() noexcept;
private:
    std::function<void()> func_;
    std::atomic<bool>  isRunning_;
    std::atomic<bool>  isExit_;
    std::string name_;
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

}//end namespace firefly

