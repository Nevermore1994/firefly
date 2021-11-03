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
#include <string_view>
#include "TimerPool.hpp"
#include "Utility.hpp"
#include "NoCopyable.hpp"

namespace firefly {

class Thread:public NoCopyable{

public:
    template<class Func, typename ... Args>
    Thread(std::string&& name, Func&& f, Args&& ... args)
        :name_(std::move(name))
        ,isRunning_(false)
        ,isExit_(false)
        ,worker_(&Thread::process, this)
        ,lastRunTimeStamp_(0)
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
        ,lastRunTimeStamp_(0)
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
    
    TimerId runAt(uint64_t timeStamp, TimerCallback func);
    TimerId runAfter(uint64_t delayTime, TimerCallback func); //ms
    
    template<class Func, typename ... Args>
    void setFunc(Func&& f, Args&& ... args) noexcept{
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
public:
    inline bool isRunning() const noexcept {
        return isRunning_.load();
    }
    
    inline Util::TimeStamp getLastRunTimeStamp() const noexcept{
        return lastRunTimeStamp_;
    }
    
    inline std::thread::id getId() const noexcept{
        return worker_.get_id();
    }
    
    inline std::string_view getName() const noexcept{
        return std::string_view (name_);
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
    Util::TimeStamp lastRunTimeStamp_ = 0;
    TimerPool timerPool_;
};

}//end namespace firefly

