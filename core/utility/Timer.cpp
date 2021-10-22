//
// Created by Nevermore on 2021/10/22.
// firefly Timer
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "Timer.hpp"
#include "Utility.hpp"

using namespace firefly;

TimerInfo::TimerInfo(uint64_t time)
    : expireTime(time)
    , timerId(Util::shortId()){

}

bool TimerInfo::operator<(const TimerInfo &rhs) const noexcept {
    return expireTime < rhs.expireTime;
}

bool TimerInfo::operator>(const TimerInfo &rhs) const noexcept {
    return expireTime > rhs.expireTime;
}

Timer::Timer()
    : timerInfo(0)
    , func(nullptr)
    , isValid(false){
    
}

Timer::Timer(uint64_t timestamp, TimerCallback f)
    : timerInfo(timestamp)
    , func(std::move(f)){
    
}

Timer::Timer(const Timer& timer) noexcept
    : timerInfo(timer.timerInfo)
    , func(timer.func)
    , isValid(timer.isValid){
    
}

Timer& Timer::operator=(const Timer& timer) noexcept{
    this->timerInfo = timer.timerInfo;
    this->isValid = timer.isValid;
    this->func = timer.func;
    return *this;
}

Timer::Timer(Timer&& timer) noexcept
        : timerInfo(timer.timerInfo)
        , func(std::move(timer.func))
        , isValid(timer.isValid){
    
}

Timer& Timer::operator=(Timer&& timer) noexcept {
    this->timerInfo = timer.timerInfo;
    this->isValid = timer.isValid;
    this->func = std::move(timer.func);
    return *this;
}

