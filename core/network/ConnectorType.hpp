//
// Created by Nevermore on 2021/11/2.
// firefly ConnectorType
// Copyright (c) 2021 Nevermore All rights reserved.
//

#pragma once

#include <cstdint>
#include "NoCopyable.hpp"
#include "IPAddress.hpp"
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

constexpr const int32_t kReadableFlag = 0x0001;
constexpr const int32_t kWriteableFlag = 0x0010;
using ConnectorID = uint64_t;

class IConnectorHandler{
public:
    virtual void reportState(Socket socket, ConnectorState state) noexcept = 0;
    virtual void reportEvent(Socket socket, ConnectorEvent event) noexcept = 0;
    virtual void reportError(Socket socket, ErrorInfo&& error) noexcept = 0;
    virtual void reportData(Socket socket, std::shared_ptr<Packet> packet) noexcept = 0;
    
    virtual void send(Socket socket) noexcept = 0;
    virtual void received(Socket socket) noexcept = 0;
};


}
