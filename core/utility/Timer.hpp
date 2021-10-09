//
// Created by 谭平 on 2021/10/8.
//

#include <cstdint>
#include <functional>

namespace firefly{

using TimerCallback = std::function<void()>;

using TimerId = uint32_t;

struct TimerInfo{
    uint64_t expireTime;
    TimerId timerId;
    
    bool operator<(const TimerInfo& rhs){
        return expireTime < rhs.expireTime;
    }
    
    TimerInfo(uint64_t time);
};

struct Timer{
    Timer(uint64_t timestamp, TimerCallback f);
    
public:
    TimerInfo timerInfo;
    TimerCallback func;
    bool  isLoop;
    bool  isValid;
};

}