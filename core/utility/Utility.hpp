//
// Created by Nevermore on 2021/10/22.
// firefly Utility
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include <string>
#include <random>

namespace firefly::Util {

//time
uint64_t nowTimestamp();

//random
template<typename T>
T random(T min, T max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dis(min, max);
    return dis(gen);
}

std::string randomString(uint32_t length);
uint64_t randomId(uint32_t length);

//id
uint32_t shortId();
std::string uuid();

//string
std::vector<std::string>& spiltString(const std::string& str, char flag, std::vector<std::string>& res, bool isSkipSpace = true);

}// end namespace firefly::Util

