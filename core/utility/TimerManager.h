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
    TimerManager(const TimerManager&&) = delete;
    TimerManager& operator=(const TimerManager&&) = delete;
    
    TimerId runAt(uint64_t timeStamp, TimerCallback func);
    TimerId runAfter(uint64_t delayTime, TimerCallback func);
    TimerId runLoop(uint64_t timeInterval, TimerManager func);

private:
    void loop();
private:
    std::unique_ptr<Thread> timerThread_;
    std::priority_queue<TimerInfo, std::vector<TimerInfo>, std::less<TimerInfo>> timerInfos_;
    std::unordered_map<TimerId, Timer> timers_;
};

}
