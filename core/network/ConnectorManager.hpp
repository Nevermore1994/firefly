//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "Connector.hpp"
#include <unordered_map>
#include <mutex>

namespace firefly::Network{

class ConnectorFactory: public IConnectorManager, std::enable_shared_from_this<ConnectorFactory>{
public:
    friend class ConnectorManager;
public:
    ~ConnectorFactory() override;
    ConnectorFactory();
    
    void reportEvent(Socket socket, ConnectorEvent event) noexcept override;
    void send(Socket socket) noexcept override;
    void received(Socket socket) noexcept override;
    void onError(Socket socket, ErrorInfo&& info) noexcept override;
    
public:
    std::shared_ptr<Connector> newConnector(std::unique_ptr<ConnectorInfo> info) noexcept;
    void removeConnector(const std::shared_ptr<Connector>& connector)noexcept;
    void removeConnector(Socket socket)noexcept;
    void removeAllConnector() noexcept;
private:
    std::unordered_map<Socket, std::shared_ptr<Connector>> connectors_;
    std::mutex mutex_;
};

class ConnectorManager{
public:
    static ConnectorFactory& shareInstance() noexcept{
        static std::once_flag flag;
        static std::shared_ptr<ConnectorFactory> factory;
        std::call_once(flag, [](){
            factory = std::make_shared<ConnectorFactory>();
        });
        return *factory;
    }
};

}



