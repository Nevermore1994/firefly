//
// Created by Nevermore on 2021/10/22.
// firefly test/testPacketPool
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "testPacketPool.h"
#include "Utility.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace firefly;

void test(uint32_t testSize){
    uint32_t size = testSize;
    vector<std::shared_ptr<Packet>> packets;
    for (int i = 0; i < 10; i++){
        std::string str = Util::randomString(size - 1);
        auto packet = PacketPool::shareInstance().newPacket((uint8_t*)str.data(), size);
        printf("packet size:%u,[%d] %s \n", size, i, packet->buffer.get());
        packets.push_back(packet);
    }
    for_each(packets.begin(), packets.end(), [](std::shared_ptr<Packet> ptr){
        PacketPool::shareInstance().releasePacket(ptr);
    });
    packets.clear();
}

void testPackPool() {
    cout << "testPackPool start" << endl;
    uint32_t size = 33;
    test(size);

    size = 177;
    test(size);

    size = 1025;
    test(size);
    PacketPool::shareInstance().release();
}
