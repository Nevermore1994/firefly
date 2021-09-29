//
//  Thread.cpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//
#include <iostream>
#include "Thread.hpp"
#include "Utility.hpp"

using namespace firefly;

Thread::Thread(const std::string& name)
    :name_(name)
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRuning_(false){
    func_ = nullptr;
}

Thread::Thread(const std::string&& name)
    :name_(name)
    ,worker_(&Thread::process, this)
    ,isExit_(false)
    ,isRuning_(false){
    func_ = nullptr;
}

Thread::~Thread() noexcept{
    if(worker_.joinable())
        worker_.join();
}

void Thread::stop() noexcept{
    isExit_ = true;
    if(worker_.joinable()){
        worker_.join();
    }
}

void Thread::pause() noexcept{
    if(!isRuning_ || isExit_){
        return;
    }
    isRuning_ = false;
}

void Thread::resume() noexcept{
    start();
}

void Thread::start() noexcept{
    if(isRuning_ || isExit_){
        return;
    }
    isRuning_ = true;
    cond_.notify_all();
}

void Thread::process() noexcept{
    while(!isExit_){
        if(isRuning_ && func_){
            func_();
        } else {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [&]{
                return isExit_ || isRuning_;
            });
        }
    }
}

