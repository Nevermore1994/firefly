//
//  Utility.hpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//

#pragma once

#include <cstdint>
#include <string>

namespace firefly {

namespace Util {

uint64_t nowTimestamp();

std::string randomString(uint32_t length);

uint64_t randomId(uint32_t length);

uint32_t shortId();

std::string uuid();

}// end namespace Util

}// end namespace firefly
