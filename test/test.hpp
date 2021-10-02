//
//  test.h
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//
#pragma once

#include "testThread.hpp"
#include "testPacketPool.h"

inline void runTest(){
    testPackPool();

    testThread();
}
