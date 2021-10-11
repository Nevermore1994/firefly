//
// Created by 谭平 on 2021/10/8.
//

#include "TimerManager.h"
#include <chrono>

using namespace firefly;
using namespace std::chrono_literals;

TimerManager::TimerManager()
    : timerThread_(std::make_unique<Thread>("timerManager", &TimerManager::loop, this)){
    timerThread_->start();
}

TimerManager::~TimerManager(){
    decltype(timerInfos_) temp;
    timerInfos_.swap(temp);
    timerThread_->stop();
}

void TimerManager::loop() {
    uint64_t now = Util::nowTimestamp();
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
    
    std::this_thread::sleep_for(5ms);
}

TimerId TimerManager::addTimer(uint64_t timestamp, TimerCallback func, bool isLoop, uint64_t timeInterval) {
    Timer timer(timestamp, std::move(func));
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

TimerId TimerManager::runAt(uint64_t timestamp, TimerCallback func) {
    return addTimer(timestamp, std::move(func), false);
}

TimerId TimerManager::runAfter(uint64_t delayTime, TimerCallback func) {
    uint64_t now = Util::nowTimestamp();
    return addTimer(now + delayTime * 1000, std::move(func), false);
}

TimerId TimerManager::runLoop(uint64_t timeInterval, TimerCallback func) {
    uint64_t now = Util::nowTimestamp();
    return addTimer(now + timeInterval * 1000, std::move(func), true, timeInterval * 1000);
}

void TimerManager::cancel(TimerId id) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if(timers_.count(id)){
            timers_[id].isValid = false;
        }
    }
}