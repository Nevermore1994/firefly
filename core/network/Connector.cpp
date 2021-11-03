//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Connector.hpp"
#include <cstdint>

using namespace firefly::Network;

Connector::Connector(ConnectorInfo info){
    
}

Connector::~Connector() {

}

bool Connector::isActive() const noexcept {
    return false;
}

bool Connector::isReadable() const noexcept {
    auto flag = static_cast<int32_t>(type);
    return (flag & kReadableFlag) > 0;
}

bool Connector::isWriteable() const noexcept {
    auto flag = static_cast<int32_t>(type);
    return (flag & kWriteableFlag) > 0;
}

void Connector::onError() {

}

void Connector::onReceived() {

}

void Connector::onSend() {

}