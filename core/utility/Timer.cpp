//
// Created by 谭平 on 2021/10/8.
//

#include "Timer.hpp"
#include "Utility.hpp"

using namespace firefly;

TimerInfo:TimerInfo(uint64_t time)
    : expireTime(time)
    , timerId(Util::shortId()){

}

Timer::Timer(uint64_t timestamp, TimerCallback f)
    : timerInfo(timestamp)
    , func(f)
    , isLoop(false)
    , isValid(true){
    
}
