//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "IConnector.hpp"
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
    
    void reportError(Socket socket, ErrorInfo &&error) override;
    void reportEvent(Socket socket, ConnectorEvent event) override;
    void reportState(Socket socket, ConnectorState state) override;
    void reportData(Socket socket, std::shared_ptr<Packet> packet) override;
private:
    ConnectorManager();
    
private:
};

}



