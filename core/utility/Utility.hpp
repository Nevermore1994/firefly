//
// Created by Nevermore on 2021/10/22.
// firefly Utility
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include <string>


namespace firefly::Util {

uint64_t nowTimestamp();

std::string randomString(uint32_t length);
uint64_t randomId(uint32_t length);

uint32_t shortId();
std::string uuid();

std::vector<std::string>& spiltString(const std::string& str, char flag, std::vector<std::string>& res, bool isSkipSpace = true);

}// end namespace firefly::Util

