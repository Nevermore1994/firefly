//
// Created by Nevermore on 2021/11/20.
// firefly Buffer
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include "NoCopyable.hpp"
#include <cstdint>
#include <mutex>
#include <array>

namespace firefly{

uint32_t  kMemoryFactor = 2;

template<typename T, uint32_t PresetBufferSize>
class Buffer:public NoCopyable{
public:
    Buffer();
    ~Buffer() = default;
public:
    void append(T* data, uint32_t size) noexcept;
    void reset() noexcept;
    uint32_t read(T* data, uint32_t size) noexcept;
    bool empty() const noexcept;
public:
    inline uint32_t readPosition() const noexcept{
        return readPosition_;
    }
    
    inline uint32_t writePosition() const noexcept{
        return writePosition_;
    }
    
    inline uint32_t capacity() const noexcept{
        return capacity_;
    }
    
    inline uint32_t size() const noexcept{
        return writePosition_ - readPosition_;
    }
    
    inline T* data() noexcept{
        return data_.get();
    }
    
    inline T* front() noexcept{
        assert(size() == 0);
        return data_.get() + readPosition_ + 1;
    }
    
    inline T* back() noexcept{
        return data_.get() + writePosition_;
    }
    
private:
    void alloc() noexcept;
private:
    std::unique_ptr<T[]> data_;
    uint32_t capacity_ = PresetBufferSize;
    uint32_t readPosition_ = 0;
    uint32_t writePosition_ = 0;
    std::mutex mutex_;
};

template<typename T, uint32_t PresetBufferSize>
Buffer<T, PresetBufferSize>::Buffer()
    :data_(std::make_unique<T[]>(PresetBufferSize)){
    
}

template<typename T, uint32_t PresetBufferSize>
void Buffer<T, PresetBufferSize>::append(T* data, uint32_t size) noexcept {
    uint32_t surplusWriteSize = capacity_ - writePosition_;
    if(surplusWriteSize < size){
        alloc();
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        memmove(data_.get(), data, size);
    }
}

template<typename T, uint32_t PresetBufferSize>
void Buffer<T, PresetBufferSize>::reset() noexcept {
    if(data_){
        std::unique_lock<std::mutex> lock(mutex_);
        if(capacity_ > PresetBufferSize){
            data_.reset(std::make_unique<T[]>(PresetBufferSize));
        }
        capacity_ = PresetBufferSize;
        writePosition_ = 0;
        readPosition_ = 0;
    }
}

template<typename T, uint32_t PresetBufferSize>
uint32_t Buffer<T, PresetBufferSize>::read(T *data, uint32_t size) noexcept {
    uint32_t surplusSize = this->size();
    uint32_t readSize = 0;
    if(surplusSize == 0){
        return readSize;
    } else if(surplusSize >= size){
        readSize = size;
    } else {
        readSize = surplusSize;
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        memmove(data, data_.get() + readSize, size);
        readPosition_ += readSize;
    }
    return readSize;
}

template<typename T, uint32_t PresetBufferSize>
void Buffer<T, PresetBufferSize>::alloc() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    decltype(data_) temp = std::make_unique<T[]>(capacity_ * kMemoryFactor);
    auto surplusSize = size();
    memmove(temp.get(), data_.get() + readPosition_, surplusSize);
    writePosition_ = surplusSize;
    data_ = std::move(temp);
}

template<typename T, uint32_t PresetBufferSize>
bool Buffer<T, PresetBufferSize>::empty() const noexcept {
    return size() == 0;
}

}



