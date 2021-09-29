//
//  Utility.cpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//

#include "Utility.hpp"
#include <chrono>

using namespace firefly;
using namespace firefly::Util;

uint64_t Util::nowTimeStamp(){
    auto tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return tp.time_since_epoch().count();
}
