//
// Created by Nevermore on 2021/10/22.
// firefly test/testTimer
// Copyright (c) 2021 Nevermore All rights reserved.
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
        cout << "test runAfter first:"<< (Util::nowTimeStamp() - now) / 1000 << endl;
    });
    auto t2 = TimerManager::shareInstance().runLoop(10, [&](){
        static int count = 0;
        auto t = Util::nowTimeStamp();
        cout << "test runLoop:"<< (t - now) / 1000 << ", count:" << ++count << endl;
        now = t;
    });
    std::this_thread::sleep_for(1000ms);
    TimerManager::shareInstance().cancel(t2);
    cout << "------ test timer t1  end:" << Util::nowTimeStamp() << endl;
    
    for(int i = 0; i < 100; i++){
        TimerManager::shareInstance().runAfter(i + 10, [i](){
            cout << "test runAfter:"<< i << " "<< Util::nowTimeStamp()  << endl;
            TimerManager::shareInstance().runAfter(i * 10, [i](){
                cout << "test runAfter2:"<< i << " "<< Util::nowTimeStamp() << endl;
            });
        });
    }
    std::this_thread::sleep_for(2000ms);
    cout << "------ test timer t2  end:" << Util::nowTimeStamp() << endl;
}