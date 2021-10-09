//
//  Thread.cpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//
#include <pthread.h>
#include "Thread.hpp"
#include "Utility.hpp"

using namespace firefly;

Thread::Thread(const std::string& name)
    :name_(name)
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRunning_(false){
    func_ = nullptr;
}

Thread::Thread(const std::string&& name)
    :name_(name)
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRunning_(false){
    func_ = nullptr;
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
        if(isRunning_ && func_){
            func_();
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
