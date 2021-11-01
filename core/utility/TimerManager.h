//
// Created by Nevermore on 2021/10/22.
// firefly TimerManager
// Copyright (c) 2021 Nevermore All rights reserved.
//


#pragma once
#include <functional>
#include <memory>
#include "Thread.hpp"
#include "Timer.hpp"
#include "TimerPool.hpp"

namespace firefly{

class TimerManager{
public:
    inline static TimerManager& shareInstance(){
        static TimerManager instance;
        return instance;
    }

public:
    TimerManager();
    ~TimerManager();
    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;
    TimerManager(TimerManager&&) = delete;
    TimerManager& operator=(TimerManager&&) = delete;
    
    TimerId runAt(uint64_t timeStamp, TimerCallback func);
    TimerId runAfter(uint64_t delayTime, TimerCallback func); //ms
    TimerId runLoop(uint64_t timeInterval, TimerCallback func);
    
    void cancel(TimerId id);

private:
    void loop();
private:
    std::unique_ptr<Thread> timerThread_;
    TimerPool pool_;
};

}
