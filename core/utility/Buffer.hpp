//
// Created by Nevermore on 2021/11/20.
// firefly Buffer
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include "NoCopyable.hpp"
#include <cstdint>
#include <mutex>
#include <vector>
#include <atomic>

namespace firefly{

template<typename T>
class Buffer:public NoCopyable{
public:
    Buffer();
    ~Buffer() = default;
public:
    void append(const T* data, uint32_t size) noexcept;
    void reset() noexcept;
    uint32_t read(T* data, uint32_t size) noexcept;
public:
    [[maybe_unused]]
    inline uint32_t readPosition() const noexcept{
        return readPosition_;
    }
    
    [[maybe_unused]]
    inline uint32_t writePosition() const noexcept{
        return writePosition_;
    }
    
    [[maybe_unused]]
    inline uint32_t length() const noexcept{
        return writePosition_ - readPosition_;
    }
    
    [[maybe_unused]]
    inline T* front() noexcept{
        std::unique_lock<std::mutex> lock(mutex_);
        return data_->data() + readPosition_;
    }
    
    [[maybe_unused]]
    inline bool empty() noexcept{
        std::unique_lock<std::mutex> lock(mutex_);
        return data_->empty();
    }
private:
    std::unique_ptr<std::vector<T>> data_;
    std::atomic<uint32_t> readPosition_  = 0;
    std::atomic<uint32_t> writePosition_ = 0;
    std::mutex mutex_;
};

template<typename T>
Buffer<T>::Buffer()
    :data_(std::make_unique<std::vector<T>>()){
    
}

template<typename T>
void Buffer<T>::append(const T* data, uint32_t size) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    data_->insert(data_->end(), data, data + size);
}

template<typename T>
void Buffer<T>::reset() noexcept {
    if(data_){
        std::unique_lock<std::mutex> lock(mutex_);
        data_->clear();
        writePosition_ = 0;
        readPosition_ = 0;
    }
}

template<typename T>
uint32_t Buffer<T>::read(T *data, uint32_t size) noexcept {
    uint32_t dataLength = length();
    uint32_t readSize = 0;
    if(dataLength == 0){
        return readSize;
    } else if(dataLength >= size){
        readSize = size;
    } else {
        readSize = dataLength;
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        memmove(data, data_->data() + readPosition_, readSize);
        readPosition_ += readSize;
    }
    return readSize;
}

}



