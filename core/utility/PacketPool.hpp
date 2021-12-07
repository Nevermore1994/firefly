//
// Created by Nevermore on 2021/10/22.
// firefly PacketPool
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <array>
#include <mutex>
#include "MemoryPool.hpp"
#include "Packet.hpp"
#include "NoCopyable.hpp"

namespace firefly {

constexpr uint32_t kPacketSize = 512;

template<>
class MemoryPool<Packet> : public NoCopyable{
    using PacketPool = MemoryPool<Packet>;
    using ChunkType = std::list<std::shared_ptr<Packet>>;
public:
    inline static MemoryPool<Packet>& shareInstance(){
        static MemoryPool<Packet> instance;
        return instance;
    }
    
public:
    ~MemoryPool<Packet>(){
        release();
    }
    
    void release(){
        std::unique_lock<std::mutex> lock(mutex_);
        for(auto& pair:pool_){
            pair.second.clear();
        }
        pool_.clear();
    }
    
    std::shared_ptr<Packet> newPacket(uint8_t* data, uint32_t len){
        std::unique_lock<std::mutex> lock(mutex_);
        std::shared_ptr<Packet> packet = nullptr;
        uint32_t typeValue = kPacketTypeExtraValue;
        if(len < kPacketTypeLittleValue && !pool_[kPacketTypeLittleValue].empty()){
            typeValue = kPacketTypeLittleValue;
        } else if(len < kPacketTypeNormalValue && !pool_[kPacketTypeNormalValue].empty()){
            typeValue = kPacketTypeNormalValue;
        } else if(len < kPacketTypeBigValue && !pool_[kPacketTypeBigValue].empty()){
            typeValue = kPacketTypeBigValue;
        }
        
        if(typeValue == kPacketTypeExtraValue){
            packet = std::make_shared<Packet>(len + 1);
        } else {
            packet = pool_[typeValue].front();
            pool_[typeValue].pop_front();
        }
        memset(packet->buffer.get(), 0, packet->capacity);
        memcpy(packet->buffer.get(), data, len);
        packet->length = len;
        return packet;
    }

    void releasePacket(std::shared_ptr<Packet> packet) noexcept{
        std::unique_lock<std::mutex> lock(mutex_);
        if(packet->type == PacketType::PacketTypeExtra){
            packet.reset(); //pointer reset, release pointer
        } else {
            auto type = static_cast<uint32_t>(packet->type);
            packet->release(); //packet reset
            pool_[type].push_back(packet);
        }
    }
    
private:
    MemoryPool<Packet>():poolSize_(kPacketSize){
        std::array<uint32_t, 3> types = {
            kPacketTypeLittleValue,
            kPacketTypeNormalValue,
            kPacketTypeBigValue
        };
        for(auto& type : types){
            for(int i = 0; i < kPacketSize; i++){
                pool_[type].emplace_back(std::make_shared<Packet>(type, static_cast<PacketType>(type)));
            }
        }
    }
private:
    std::unordered_map<uint32_t, ChunkType> pool_;
    uint32_t poolSize_;
    std::mutex mutex_;
};

using PacketPool = MemoryPool<Packet>;

}
