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

uint64_t nowTimeStamp();

std::string randomString(uint32_t length);

}// end namespace Util

}// end namespace firefly
