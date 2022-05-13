//
// Created by Nevermore on 2022/5/13.
// firefly Reflection
// Copyright (c) 2022 Nevermore All rights reserved.
//

#include "Reflection.hpp"

using namespace firefly;

template<typename T>
void firefly::registerClass(const std::string& name) {
    return Reflection::shareInstance().enrolment(name, []() {
        return new T();
    });
}

void* firefly::generate(const std::string& name) {
    return Reflection::shareInstance().generate(name)();
}
