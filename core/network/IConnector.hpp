//
// Created by Nevermore on 2021/11/2.
// firefly IConnector
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include <cstdint>
#include "NoCopyable.hpp"

namespace firefly::Network{

enum class ConnectorType{
    Read = 0x0001,
    Write = 0x0010,
    ReadWrite = 0x011,
};

constexpr const int32_t kReadableFlag = 0x0001;
constexpr const int32_t kWriteableFlag = 0x0010;

class IConnector:public NoCopyable{
public:
    IConnector() = default;
    ~IConnector() = default;

public:
    virtual bool isActive() const noexcept = 0;
    virtual bool isReadable() const noexcept = 0;
    virtual bool isWriteable() const noexcept = 0;
    virtual void onReceived() = 0;
    virtual void onError() = 0;
    virtual void onSend() = 0;
};

}
