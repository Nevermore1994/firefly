//
// Created by Nevermore on 2021/10/22.
// firefly MemoryPool
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once
#include <cstdint>
#include <unordered_map>
#include <list>
#include <memory>
#include <algorithm>

namespace firefly {

template<class T, uint32_t BlockSize = 512>
class MemoryPool{
    using ChunkType = std::list<std::shared_ptr<T>>;
public:
    MemoryPool()
        :size_(BlockSize){
        
    }
    
    MemoryPool(const MemoryPool&) = delete;
    
    MemoryPool(const MemoryPool&&) = delete;
    
    MemoryPool& operator=(const MemoryPool&) = delete;
    
    MemoryPool& operator=(const MemoryPool&&) = delete;
    
    ~MemoryPool(){
        release();
    }
    
    void release(){
        for(auto& pair:pool){
            pair.second.clear();
        }
        pool.clear();
    }
private:
    std::unordered_map<uint32_t, ChunkType> pool;
    uint32_t size_;
};

}// end of firefly