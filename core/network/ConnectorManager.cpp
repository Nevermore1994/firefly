//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "ConnectorManager.hpp"

 using namespace firefly::Network;

ConnectorManager::~ConnectorManager() {
    
}

ConnectorManager::ConnectorManager() {

}

void ConnectorManager::reportError(Socket socket, ErrorInfo&& error) noexcept {

}

void ConnectorManager::reportEvent(Socket socket,ConnectorEvent event) noexcept {
    
}

void ConnectorManager::reportState(Socket socket,ConnectorState state) noexcept{

}

void ConnectorManager::reportData(Socket socket, std::shared_ptr<Packet> packet) noexcept {
    
}

void ConnectorManager::send(Socket socket) noexcept {

}

void ConnectorManager::received(Socket socket) noexcept {

}
