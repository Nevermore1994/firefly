//
// Created by Nevermore on 2021/11/3.
// firefly NoCopyable
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

namespace firefly{

class NoCopyable{
public:
    NoCopyable() = default;
    virtual ~NoCopyable() = default;
    NoCopyable(const NoCopyable&) = delete;
    NoCopyable& operator=(const NoCopyable&) = delete;
    NoCopyable(NoCopyable&&) = delete;
    NoCopyable& operator=(NoCopyable&&) = delete;
};

}
