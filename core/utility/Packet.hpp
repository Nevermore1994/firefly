//
// Created by Nevermore on 2021/10/22.
// firefly MemoryPool
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include <cstring>

namespace firefly {

constexpr uint16_t kPacketTypeLittleValue = 256;
constexpr uint16_t kPacketTypeNormalValue = 512;
constexpr uint16_t kPacketTypeBigValue = 1504; //MTU Max value
constexpr uint16_t kPacketTypeExtraValue = 7678; //rand number

enum class PacketType{
    PacketTypeLittle = kPacketTypeLittleValue,
    PacketTypeNormal = kPacketTypeNormalValue,
    PacketTypeBig = kPacketTypeBigValue,
    PacketTypeExtra = kPacketTypeExtraValue,
};


struct Packet{
    std::unique_ptr<uint8_t[]> buffer;
    uint8_t pos; //start position
    uint8_t length; //data len
    uint8_t size; // buffer size
    PacketType type;
    uint64_t timeStamp;
    
    Packet()
        :buffer(nullptr)
        ,pos(0)
        ,length(0)
        ,size(0)
        ,type(PacketType::PacketTypeExtra)
        ,timeStamp(0){
        
    }
    
    Packet(uint32_t size)
        :buffer(std::make_unique<uint8_t[]>(size))
        ,pos(0)
        ,length(0)
        ,size(size)
        ,type(PacketType::PacketTypeExtra)
        ,timeStamp(0){
    }

    Packet(uint32_t size, PacketType type)
        :buffer(std::make_unique<uint8_t[]>(size))
        ,pos(0)
        ,length(0)
        ,size(size)
        ,type(type)
        ,timeStamp(0){
    }

    Packet(Packet& packet){
        this->type = packet.type;
        this->pos = packet.pos;
        this->buffer = std::move(packet.buffer);
        this->length = packet.length;
        this->timeStamp = packet.timeStamp;
        this->size = packet.size;
        packet.buffer.reset();
    }

    Packet(Packet&& packet){
        this->type = packet.type;
        this->pos = packet.pos;
        this->buffer = std::move(packet.buffer);
        this->length = packet.length;
        this->timeStamp = packet.timeStamp;
        this->size = packet.size;
        packet.buffer.reset(nullptr);
    }

    inline void release(){
        memset(this->buffer.get(), 0, size);
        this->pos = 0;
        this->length = 0;
        this->timeStamp = 0;
    }
};

}
