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
#include <cassert>

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
    uint16_t pos; //start position
    uint16_t length; //data len
    uint16_t capacity; // buffer capacity
    PacketType type;
    uint64_t timeStamp;
    
    Packet()
        : buffer(nullptr)
        , pos(0)
        , length(0)
        , capacity(0)
        , type(PacketType::PacketTypeExtra)
        , timeStamp(0){
        
    }
    
    explicit Packet(uint32_t size)
        : buffer(std::make_unique<uint8_t[]>(size))
        , pos(0)
        , length(0)
        , capacity(size)
        , type(PacketType::PacketTypeExtra)
        , timeStamp(0){
    }

    Packet(uint32_t size, PacketType type)
        : buffer(std::make_unique<uint8_t[]>(size))
        , pos(0)
        , length(0)
        , capacity(size)
        , type(type)
        , timeStamp(0){
    }

    Packet(Packet& packet) noexcept{
        this->type = packet.type;
        this->pos = packet.pos;
        this->buffer = std::move(packet.buffer);
        this->length = packet.length;
        this->timeStamp = packet.timeStamp;
        this->capacity = packet.capacity;
    }

    Packet(Packet&& packet) noexcept {
        this->type = packet.type;
        this->pos = packet.pos;
        this->buffer = std::move(packet.buffer);
        this->length = packet.length;
        this->timeStamp = packet.timeStamp;
        this->capacity = packet.capacity;
    }

    inline void release() noexcept{
        memset(this->buffer.get(), 0, capacity);
        this->pos = 0;
        this->length = 0;
        this->timeStamp = 0;
    }
    
    inline bool isValid() const noexcept{
        return (length - pos) > 0;
    }
    
    inline uint8_t* front() const noexcept{
        assert(capacity > 0 && pos < length);
        if(capacity > 0 && pos < length){
            return buffer.get() + pos;
        }
        return nullptr;
    }
    
    inline void freed(uint32_t moveLength) noexcept{
        assert(pos + moveLength < length);
        if(pos + moveLength < length){
            pos += moveLength;
        }
    }
    
    inline void move(uint32_t moveLength) noexcept{
        assert(moveLength < length);
        if(moveLength < length){
            pos = moveLength;
        }
    }
    
    inline uint16_t size() const noexcept{
        if(length >= pos){
            return length - pos;
        }
        return 0;
    }
};

}
