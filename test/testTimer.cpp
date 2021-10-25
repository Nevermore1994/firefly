//
// Created by 谭平 on 2021/10/11.
//

#include "testTimer.hpp"
#include <iostream>

using namespace std;
using namespace firefly;
using namespace chrono_literals;


void testTimer(){
    uint64_t now = Util::nowTimeStamp();
    cout << "test timer start:" << now << endl;
    TimerId t = TimerManager::shareInstance().runAfter(7, [&](){
        cout << "test runAfter:"<< (Util::nowTimeStamp() - now) / 1000 << endl;
    });
    auto t2 = TimerManager::shareInstance().runLoop(10, [&](){
        static int count = 0;
        auto t = Util::nowTimeStamp();
        cout << "test runLoop:"<< (t - now) / 1000 << ", count:" << ++count << endl;
        now = t;
    });
    std::this_thread::sleep_for(1000ms);
    TimerManager::shareInstance().cancel(t2);
    cout << "test timer end:" << Util::nowTimeStamp() << endl;
}