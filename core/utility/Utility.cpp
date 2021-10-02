//
//  Utility.cpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//

#include "Utility.hpp"
#include <chrono>
#include <random>

using namespace firefly;
using namespace firefly::Util;

uint64_t Util::nowTimeStamp(){
    auto tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return tp.time_since_epoch().count();
}

std::string Util::randomString(uint32_t length) {
    static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string result;
    result.resize(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 61);// 61 = 26 * 2 + 10 - 1. [0, 61]
    for (int i = 0; i < length; i++)
        result[i] = charset[dis(gen)];
    return result;
}
