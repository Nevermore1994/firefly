//
// Created by Nevermore on 2021/10/22.
// firefly TimerManager
// Copyright (c) 2021 Nevermore All rights reserved.
//


#include "TimerManager.h"
#include <chrono>

using namespace firefly;
using namespace std::chrono_literals;

TimerManager::TimerManager()
    : timerThread_(std::make_unique<Thread>("TimerManager", &TimerManager::loop, this)){
    timerThread_->start();
}

void TimerManager::loop() {
    pool_.loop();
    std::this_thread::sleep_for(1ms);
}

TimerId TimerManager::runAt(uint64_t timeStamp, TimerCallback func) {
    return pool_.runAt(timeStamp, std::move(func));
}

TimerId TimerManager::runAfter(uint64_t delayTime, TimerCallback func) {
    return pool_.runAfter(delayTime, std::move(func));
}

TimerId TimerManager::runLoop(uint64_t timeInterval, TimerCallback func) {
    return pool_.runLoop(timeInterval, std::move(func));
}

void TimerManager::cancel(TimerId id) {
    pool_.cancel(id);
}

void TimerManager::release() noexcept {
    pool_.clear();
    timerThread_->stop();
}
