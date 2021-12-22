//
// Created by Nevermore on 2021/12/22.
// firefly Time
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Time.hpp"

using namespace firefly::Time;

TimeStamp firefly::Time::nowTimeStamp(){
    auto tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return tp.time_since_epoch().count();
}