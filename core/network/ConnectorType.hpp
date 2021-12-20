//
// Created by Nevermore on 2021/11/2.
// firefly ConnectorType
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include "NoCopyable.hpp"
#include "NetworkType.hpp"
#include "Type.hpp"
#include "Packet.hpp"

namespace firefly::Network{

enum class ConnectorType{
    Read = 0x0001,
    Write = 0x0010,
    ReadWrite = 0x011,
};

enum class ConnectorState{
    Unknown = 0,
    Init,
    Connecting,
    Connected,
    Disconnected,
    Error
};

enum class ConnectorEvent{
    Add,
    Remove,
};

constexpr const int32_t kReadableFlag = static_cast<int32_t>(ConnectorType::Read);
constexpr const int32_t kWriteableFlag = static_cast<int32_t>(ConnectorType::Write);
using ConnectorID = uint64_t;

inline bool isConnectorReadable(ConnectorType type){
    auto flag = static_cast<int32_t>(type);
    return (flag & kReadableFlag) > 0;
}

inline bool isConnectorWriteable(ConnectorType type){
    auto flag = static_cast<int32_t>(type);
    return (flag & kWriteableFlag) > 0;
}

}
