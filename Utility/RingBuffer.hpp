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
#include <atomic>

namespace firefly {

constexpr uint64_t RingBufferLength = 1024 * 1024 * 500;

template<typename T, uint64_t Size>
class RingBuffer : public NoCopyable {
public:
    RingBuffer();
    
    ~RingBuffer() override = default;

public:
    void append(const T *data, uint64_t size) noexcept;
    
    void reset() noexcept;
    
    uint64_t read(T *data, uint64_t size) noexcept;
    
    uint64_t maxSize() const noexcept{
        return size_;
    }

private:
    inline uint64_t length() const noexcept {
        uint64_t length = writePosition_ - readPosition_;
        if(length < 0){
            length += size_;
        }
        return writePosition_ - readPosition_;
    }

private:
    std::unique_ptr<std::array<T, Size>> data_;
    uint64_t readPosition_ = 0;
    uint64_t writePosition_ = 0;
    uint64_t size_ = Size;
};

template<typename T, uint64_t Size>
RingBuffer<T, Size>::RingBuffer()
    :data_(std::make_unique<std::array<T, Size>>())
    ,size_(Size){
    
}

template<typename T, uint64_t Size>
void RingBuffer<T, Size>::append(const T *data, uint64_t size) noexcept {
    auto pos = writePosition_ + size;
    if(pos > size_){
        auto t = size_ - writePosition_;
        memcpy(data_->data() + writePosition_, data, t);
        writePosition_ = 0;
        memcpy(data_->data() + writePosition_, data + t, size - t);
        writePosition_ = size - t;
    } else {
        memcpy(data_->data() + writePosition_, data, size);
        writePosition_ = pos;
    }
}

template<typename T, uint64_t Size>
void RingBuffer<T, Size>::reset() noexcept {
    if(data_) {
        writePosition_ = 0;
        readPosition_ = 0;
    }
}

template<typename T, uint64_t Size>
uint64_t RingBuffer<T, Size>::read(T *data, uint64_t size) noexcept {
    uint32_t dataLength = length();
    uint32_t readSize = 0;
    if(dataLength == 0) {
        return readSize;
    } else if(dataLength >= size) {
        readSize = size;
    } else {
        readSize = dataLength;
    }
    
    auto pos = readPosition_ + readSize;
    if(readSize < size_){
        memcpy(data, data_->data() + readPosition_, readSize);
        readPosition_ += readSize;
    } else {
        auto temp = size_ - readPosition_;
        memcpy(data, data_->data() + readPosition_, temp);
        readPosition_ = 0;
        memcpy(data + temp, data_->data() + readPosition_, readSize - temp);
        readPosition_ =  readSize - temp;
    }
    return readSize;
}

}



