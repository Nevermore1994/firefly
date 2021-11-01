//
// Created by Nevermore on 2021/10/25.
// firefly TimerPool
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>
#include <mutex>
#include "Timer.hpp"

namespace firefly{

class TimerPool{
public:
    TimerPool() = default;
    ~TimerPool();
    TimerPool(const TimerPool&) = delete;
    TimerPool& operator=(const TimerPool&) = delete;
    TimerPool(TimerPool&&) noexcept ;
    TimerPool& operator=(TimerPool&&) noexcept ;
    
    TimerId runAt(uint64_t timeStamp, TimerCallback func);
    TimerId runAfter(uint64_t delayTime, TimerCallback func); //ms
    TimerId runLoop(uint64_t timeInterval, TimerCallback func);
    
    void cancel(TimerId id);
    void loop() noexcept;
    void clear() noexcept;
private:
    TimerId addTimer(uint64_t timeStamp, TimerCallback func, bool isLoop, uint64_t timeInterval = 0);
private:
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::greater<>> timerInfos_;
    std::unordered_map<TimerId, Timer> timers_;
    std::mutex mutex_;
};

}
