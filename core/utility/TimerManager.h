//
// Created by 谭平 on 2021/10/8.
//

#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>
#include "Thread.hpp"
#include "Timer.hpp"

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
    
    TimerId runAt(uint64_t timestamp, TimerCallback func);
    TimerId runAfter(uint64_t delayTime, TimerCallback func); //ms
    TimerId runLoop(uint64_t timeInterval, TimerCallback func);
    
    void cancel(TimerId id);
private:
    void loop();
    TimerId addTimer(uint64_t timestamp, TimerCallback func, bool isLoop, uint64_t timeInterval = 0);
private:
    std::unique_ptr<Thread> timerThread_;
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::greater<>> timerInfos_;
    std::unordered_map<TimerId, Timer> timers_;
    std::mutex mutex_;
};

}
