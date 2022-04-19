//
// Created by Nevermore on 2021/12/22.
// firefly Time
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <chrono>

namespace firefly::Time {

using namespace std::chrono;

//time
using TimeStamp = uint64_t;

TimeStamp nowTimeStamp();

} //end namespace firefly::Time

