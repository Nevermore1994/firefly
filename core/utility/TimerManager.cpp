//
// Created by 谭平 on 2021/10/8.
//

#include "TimerManager.h"

using namespace firefly;

TimerManager::TimerManager()
    : timerThread_(std::make_unique<Thread>("timerManager", &TimerManager::loop, this)){
    
}

TimerManager::~TimerManager(){
    decltype(timerInfos_) temp;
    timerInfos_.swap(temp);
    timerThread_->stop();
}

void TimerManager::loop() {
    
}