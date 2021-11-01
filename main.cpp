//
//  main.cpp
//  firefly
//
//  Created by Nevermore on 2021/9/29.
//

#include "test.hpp"
#include <netdb.h>
#include "Log.hpp"

using namespace firefly;
int main(int argc, const char * argv[]) {
    runTest();
    logd("test log");
    return 0;
}
