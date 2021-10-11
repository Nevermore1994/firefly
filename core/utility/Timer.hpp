//
// Created by 谭平 on 2021/10/8.
//

#pragma once

#include <cstdint>
#include <functional>

namespace firefly{

using TimerCallback = std::function<void()>;

using TimerId = uint32_t;

struct TimerInfo{
    uint64_t expireTime;
    TimerId timerId;
    
    bool isLoop = false;
    uint64_t loopInterval = 0; //ms
    
    bool operator<(const TimerInfo& rhs) const noexcept;
    
    TimerInfo(uint64_t time);
    TimerInfo(const TimerInfo& info) = default;
    TimerInfo& operator=(const TimerInfo& info) = default;
    TimerInfo(TimerInfo&& info) = default;
    TimerInfo& operator=(TimerInfo&& info) = default;
};

struct Timer{
    Timer();
    Timer(uint64_t timestamp, TimerCallback f);
    Timer(const Timer& timer) noexcept;
    Timer& operator=(const Timer& timer) noexcept;
    Timer(Timer&& timer) noexcept;
    Timer& operator=(Timer&& timer) noexcept;
public:
    TimerInfo timerInfo;
    TimerCallback func;
    bool isValid = true;
};

}