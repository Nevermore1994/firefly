//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "Connector.hpp"
#include <unordered_map>

namespace firefly::Network{

class ConnectorManager: public IConnectorHandler{
public:
    static inline ConnectorManager& shareInstance() {
        static ConnectorManager instance;
        return instance;
    }

public:
    ~ConnectorManager();
    
    void reportError(Socket socket, ErrorInfo &&error) noexcept override;
    void reportEvent(Socket socket, ConnectorEvent event) noexcept override;
    void reportState(Socket socket, ConnectorState state) noexcept override;
    void reportData(Socket socket, std::shared_ptr<Packet> packet) noexcept override;
    void send(Socket socket) noexcept override;
    void received(Socket socket) noexcept override;
private:
    ConnectorManager();
private:
    std::unordered_map<Socket, std::shared_ptr<Connector>> connectors_;
};

}



