//
// Created by Nevermore on 2021/10/22.
// firefly Thread
// Copyright (c) 2021 Nevermore All rights reserved.
//

#ifndef __APPLE__
#include <pthread.h>
#endif
#include "Thread.hpp"

using namespace firefly;

Thread::Thread(const std::string& name)
    :name_(name)
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRunning_(false)
    ,lastRunTimeStamp_(0){
    func_ = nullptr;
}

Thread::Thread(std::string&& name)
    :name_(std::move(name))
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRunning_(false)
    ,lastRunTimeStamp_(0)
    ,func_(nullptr){
}

Thread::~Thread() noexcept{
    stop();
}

void Thread::stop() noexcept{
    if(isExit_){
        return;
    }
    isExit_ = true;
    cond_.notify_all();
    if(worker_.joinable()){
        worker_.join();
    }
}

void Thread::pause() noexcept{
    if(!isRunning_ || isExit_){
        return;
    }
    isRunning_ = false;
}

void Thread::resume() noexcept{
    start();
}

void Thread::start() noexcept{
    if(isRunning_ || isExit_){
        return;
    }
    isRunning_ = true;
    cond_.notify_all();
}

void Thread::process() noexcept{
    setThreadName();
    while(!isExit_){
        if(isRunning_){
            lastRunTimeStamp_ = Util::nowTimeStamp();
            timerPool_.loop();
            if(func_){
                func_();
            }
        } else {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [&]{
                return isExit_ || isRunning_;
            });
        }
    }
}


void Thread::setThreadName() noexcept{
#ifndef __APPLE__
    auto handle = worker_.native_handle();
    pthread_setname_np(handle,name_.c_str());
#else
    pthread_setname_np(name_.c_str());
#endif
}

TimerId Thread::runAt(uint64_t timeStamp, TimerCallback func) {
    return timerPool_.runAt(timeStamp, std::move(func));
}

TimerId Thread::runAfter(uint64_t delayTime, TimerCallback func) {
    return timerPool_.runAfter(delayTime, std::move(func));
}

