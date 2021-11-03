//
// Created by Nevermore on 2021/10/22.
// firefly Utility
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include <string>
#include <random>
#include <type_traits>

namespace firefly::Util {

//time
using TimeStamp = uint64_t;
TimeStamp nowTimeStamp();

//random
template<typename T>
T random(T min, T max){
    std::random_device rd;
    std::mt19937 gen(rd());
    if(std::is_floating_point_v<T>){
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    } else {
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }
}

template<typename T>
std::vector<T> reservoirSampling(const std::vector<T>& source, uint32_t count){
    if(count >= source.size()){
        return {source.begin(), source.end()};
    }
    std::vector<T> res(source.begin(), source.begin() + count);
    //C++20 replace range
    for(uint32_t i = count; i < source.size(); ++i){
        auto t = random<int>(0, static_cast<int>(i));
        if(t < count){
            res[t] = source[i];
        }
    }
    return res;
}

std::string randomString(uint32_t length);
uint64_t randomId(uint8_t length);

//id
uint32_t shortId();
std::string uuid();

//string
std::vector<std::string>& spiltString(const std::string& str, char flag, std::vector<std::string>& res, bool isSkipSpace = true);

}// end namespace firefly::Util

