//
// Created by Nevermore on 2021/10/22.
// firefly test/test
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include "testThread.hpp"
#include "testTimer.hpp"

using namespace firefly;

inline void runTest() {
    testThread();
    testTimer();
    //testFile();
}
