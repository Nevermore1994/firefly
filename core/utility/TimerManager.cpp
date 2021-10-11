//
// Created by 谭平 on 2021/10/8.
//

#include "TimerManager.h"
#include <chrono>

using namespace firefly;
using namespace std::chrono_literals;

TimerManager::TimerManager()
    : timerThread_(std::make_unique<Thread>("timerManager", &TimerManager::loop, this)){
    
}

TimerManager::~TimerManager(){
    decltype(timerInfos_) temp;
    timerInfos_.swap(temp);
    timerThread_->stop();
}

void TimerManager::loop() {
    uint64_t now = Util::nowTimestamp();
    std::vector<TimerInfo> expireTimeInfos;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while(now >= timerInfos_.top().expireTime){
            expireTimeInfos.push_back(timerInfos_.top());
            timerInfos_.pop();
            TimerId id = expireTimeInfos.back().timerId;
            if(timers_.count(id) > 0 && timers_[id].isValid && timers_[id].timerInfo.isLoop){
                TimerInfo timerInfo = timers_[id].timerInfo;
                timerInfo.expireTime = now + timerInfo.loopInterval;
                timerInfos_.push(timerInfo);
            }
        }
    }
    
    for(auto& timerInfo:expireTimeInfos){
        if(timers_.count(timerInfo.timerId) ==  0){
            timers_.erase(timerInfo.timerId);
            continue;
        }
        auto& timer = timers_[timerInfo.timerId];
        if(timer.isValid){
            timer.func();
        }
        
        if(!timer.timerInfo.isLoop){
            timers_.erase(timerInfo.timerId);
        }
    }
    std::this_thread::sleep_for(100ms);
}

TimerId TimerManager::addTimer(uint64_t timestamp, TimerCallback func, bool isLoop, uint64_t timeInterval) {
    Timer timer(timestamp, func);
    timer.timerInfo.isLoop = isLoop;
    timer.timerInfo.loopInterval = timeInterval;
    
    {
        std::unique_lock<std::mutex> lock(mutex_);
        timerInfos_.push(timer.timerInfo);
    }
    
    TimerId id = timer.timerInfo.timerId;
    timers_.insert(std::make_pair(id, std::move(timer)));
    return id;
}

TimerId TimerManager::runAt(uint64_t timestamp, TimerCallback func) {
    return addTimer(timestamp, func, false);
}

TimerId TimerManager::runAfter(uint64_t delayTime, TimerCallback func) {
    uint64_t now = Util::nowTimestamp();
    return addTimer(now + delayTime * 1000, func, false);
}

TimerId TimerManager::runLoop(uint64_t timeInterval, TimerCallback func) {
    uint64_t now = Util::nowTimestamp();
    return addTimer(now + timeInterval * 1000, func, true, timeInterval);
}