//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorFactory
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "ConnectorManager.hpp"
#include "NetEngine.hpp"
#include "Log.hpp"

using namespace firefly::Network;

ConnectorFactory::~ConnectorFactory() {
    removeAllConnector();
}

ConnectorFactory::ConnectorFactory() {
    NetEngine::shareInstance().setHandler(weak_from_this());
}

void ConnectorFactory::reportEvent(Socket socket, ConnectorEvent event) noexcept {
    if(connectors_.count(socket) == 0){
        loge("error, socket is invalid. %d", socket);
        return;
    }
    auto connector = connectors_[socket];
    if(ConnectorEvent::Add == event){
        NetEngine::shareInstance().add(socket, connector->getInfo().connectorType);
    } else if(ConnectorEvent::Remove == event){
        NetEngine::shareInstance().remove(socket);
    }
}

void ConnectorFactory::send(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket)){
        connectors_[socket]->onSend();
    }
}

void ConnectorFactory::received(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket)){
        connectors_[socket]->onReceived();
    }
}

void ConnectorFactory::onError(Socket socket, ErrorInfo&& info) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket)){
        connectors_[socket]->onError(std::move(info));
    }
}

std::shared_ptr<Connector> ConnectorFactory::newConnector(std::unique_ptr<ConnectorInfo> info) noexcept {
    auto connector = std::make_shared<Connector>(std::move(info));
    {
        std::unique_lock<std::mutex> lock(mutex_);
        connectors_.insert({connector->getSocket(), connector});
    }
    return connector;
}

void ConnectorFactory::removeConnector(const std::shared_ptr<Connector>& connector) noexcept{
    std::unique_lock<std::mutex> lock(mutex_);
    connectors_.erase(connector->getSocket());
}

void ConnectorFactory::removeConnector(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    connectors_.erase(socket);
}

void ConnectorFactory::removeAllConnector() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    connectors_.clear();
}
