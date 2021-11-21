//
// Created by Nevermore on 2021/11/11.
// firefly Type
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <cstdint>
#include <string>

namespace firefly{

constexpr int32_t kInvalid = -1;

struct ErrorInfo{
    std::string errorInfo;
    uint32_t errorNumber;
};

}
