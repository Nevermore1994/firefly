//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "IConnector.hpp"
#include <IPAddress.hpp>
#include <memory>

namespace firefly::Network{

struct ConnectorInfo{
    bool isBindPort{};
    IPAddressInfo localIP;
    Port localPort{};
    IPAddressInfo remoteIP;
    Port remotePort{};
    ConnectorType type;
    
    ConnectorInfo()
        :isBindPort(false)
        ,localIP()
        ,localPort()
        ,remoteIP()
        ,remotePort()
        ,type(ConnectorType::ReadWrite){
        
    }
};

class Connector:public IConnector{
public:
    Connector(ConnectorInfo info);
    ~Connector();

public:
    bool isActive() const noexcept override;
    bool isReadable() const noexcept override;
    bool isWriteable() const noexcept override;
    void onReceived() override;
    void onError() override;
    void onSend() override;
private:

private:
    std::unique_ptr<ConnectorInfo> info_;
};

}


