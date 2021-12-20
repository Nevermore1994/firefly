//
// Created by Nevermore on 2021/12/7.
// firefly IConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "Packet.hpp"
#include "ConnectorType.hpp"

namespace firefly::Network {

class IConnectorManager {
public:
    virtual void reportEvent(Socket socket, ConnectorEvent event) noexcept = 0;
    virtual void send(Socket socket) noexcept = 0;
    virtual void received(Socket socket) noexcept = 0;
    virtual void onError(Socket socket, ErrorInfo&& info) noexcept = 0;
    virtual ~IConnectorManager() = default;
};

class IConnectorHandler {
public:
    virtual void reportState(ConnectorState state) noexcept = 0;
    virtual void reportError(ErrorInfo&& error) noexcept = 0;
    virtual void reportData(std::shared_ptr<Packet> packet) noexcept = 0;
    virtual ~IConnectorHandler() = default;
};

}
