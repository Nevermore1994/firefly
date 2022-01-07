//
// Created by Nevermore on 2021/12/7.
// firefly Link
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Link.hpp"
#include "Log.hpp"
#include "ConnectorManager.hpp"
#include "PacketPool.hpp"
#include "DnsParserManager.hpp"

using namespace firefly::Network;

Link::Link(const LinkInfo& info)
    : linkInfo_(info) {
    
}

Link::Link(const LinkInfo& info, std::weak_ptr<ILinkHandler> handler)
    : linkInfo_(info)
    , handler_(std::move(handler)) {
    
}

void Link::setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept {
    handler_ = std::move(handler);
}

void Link::reportData(std::shared_ptr<Packet> packet) noexcept {
    if(handler_.expired()) {
        loge("link receive data, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler) {
        handler->onDataReceived(reinterpret_cast<const char *>(packet->front()), packet->size(), Time::nowTimeStamp());
        PacketPool::shareInstance().releasePacket(packet);
    }
}

void Link::reportError(ErrorInfo&& error) noexcept {
    if(handler_.expired()) {
        loge("link receive data, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler) {
        handler->onError(std::move(error));
    }
}

void Link::reportState(ConnectorState state) noexcept {
    if(handler_.expired()) {
        loge("link report state, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler) {
        handler->onStateChanged(state);
    }
}

bool Link::open() noexcept {
    auto info = std::make_unique<ConnectorInfo>();
    info->connectorType = ConnectorType::ReadWrite;
    info->remoteIP = linkInfo_.remoteAdderInfo;
    connector_ = ConnectorManager::shareInstance().newConnector(std::move(info));
    return true;
}

bool Link::open(SocketAddressInfo info) noexcept {
    auto connectorInfo = std::make_unique<ConnectorInfo>();
    connectorInfo->connectorType = ConnectorType::ReadWrite;
    connectorInfo->remoteIP = linkInfo_.remoteAdderInfo;
    connectorInfo->localIP = info;
    connectorInfo->isBindPort = true;
    connector_ = ConnectorManager::shareInstance().newConnector(std::move(connectorInfo));
    return true;
}

void Link::close() noexcept {
    if(connector_) {
        connector_->close();
    }
}

ConnectorState Link::state() const noexcept {
    if(connector_) {
        return connector_->state();
    }
    return ConnectorState::Unknown;
}


