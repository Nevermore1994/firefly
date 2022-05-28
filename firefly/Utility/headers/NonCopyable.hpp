//
// Created by Nevermore on 2021/11/3.
// firefly NonCopyable
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

namespace firefly {

class NonCopyable {
public:
    NonCopyable() = default;
    
    virtual ~NonCopyable() = default;
    
    NonCopyable(const NonCopyable&) = delete;
    
    NonCopyable& operator=(const NonCopyable&) = delete;
    
    NonCopyable(NonCopyable&&) = delete;
    
    NonCopyable& operator=(NonCopyable&&) = delete;
};

}
