//
// Created by Nevermore on 2021/10/25.
// firefly TimerPool
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "TimerPool.hpp"
#include "Utility.hpp"
#include <chrono>

using namespace firefly;
using namespace std::chrono_literals;

TimerPool::~TimerPool(){
    std::unique_lock<std::mutex> lock(mutex_);
    clear();
}

TimerPool::TimerPool(TimerPool&& lhs) noexcept {
    clear();
    timerInfos_.swap(lhs.timerInfos_);
    timers_.swap(lhs.timers_);
}

TimerPool& TimerPool::operator=(TimerPool&& lhs) noexcept {
    clear();
    timerInfos_.swap(lhs.timerInfos_);
    timers_.swap(lhs.timers_);
    return *this;
}

void TimerPool::clear() noexcept {
    decltype(timerInfos_) temp;
    timerInfos_.swap(temp);
    timers_.clear();
}

void TimerPool::loop() noexcept {
    uint64_t now = Util::nowTimeStamp();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(!timerInfos_.empty() && now >= timerInfos_.top().expireTime){
            auto expireTimeInfo = timerInfos_.top();
            timerInfos_.pop();
            TimerId id = expireTimeInfo.timerId;
            if(timers_.count(id) > 0 && timers_[id].isValid && timers_[id].timerInfo.isLoop){
                TimerInfo timerInfo = timers_[id].timerInfo;
                timerInfo.expireTime = now + timerInfo.loopInterval;
                timerInfos_.push(timerInfo);
            }
            
            if(timers_.count(id) ==  0){
                timers_.erase(id);
                continue;
            }
            auto& timer = timers_[id];
            if(timer.isValid){
                timer.func();
            }
            
            if(!timer.timerInfo.isLoop){
                timers_.erase(id);
            }
        }
    }
}

TimerId TimerPool::addTimer(uint64_t timeStamp, TimerCallback func, bool isLoop, uint64_t timeInterval) {
    Timer timer(timeStamp, std::move(func));
    timer.timerInfo.isLoop = isLoop;
    timer.timerInfo.loopInterval = timeInterval;
    TimerId id = timer.timerInfo.timerId;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        timerInfos_.push(timer.timerInfo);
        timers_.insert(std::make_pair(id, std::move(timer)));
    }
    return id;
}

TimerId TimerPool::runAt(uint64_t timeStamp, TimerCallback func) {
    return addTimer(timeStamp, std::move(func), false);
}

TimerId TimerPool::runAfter(uint64_t delayTime, TimerCallback func) {
    uint64_t now = Util::nowTimeStamp();
    return addTimer(now + delayTime * 1000, std::move(func), false);
}

TimerId TimerPool::runLoop(uint64_t timeInterval, TimerCallback func) {
    uint64_t now = Util::nowTimeStamp();
    return addTimer(now + timeInterval * 1000, std::move(func), true, timeInterval * 1000);
}

void TimerPool::cancel(TimerId id) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if(timers_.count(id)){
            timers_[id].isValid = false;
        }
    }
}