//
// Created by Nevermore on 2021/12/7.
// firefly Link
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Link.hpp"
#include "Log.hpp"
#include "ConnectorManager.hpp"
#include "PacketPool.hpp"

using namespace firefly::Network;

Link::Link(const LinkInfo& info)
    : linkInfo_(info){
    
}

Link::Link(const LinkInfo& info, std::weak_ptr<ILinkHandler> handler)
    : linkInfo_(info)
    , handler_(std::move(handler)){

}

void Link::setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept {
    handler_ = std::move(handler);
}

void Link::reportData(std::shared_ptr<Packet> packet) noexcept {
    if(handler_.expired()){
        loge("link receive data, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler){
        handler->onDataReceived(reinterpret_cast<const char*>(packet->front()), packet->size(), Util::nowTimeStamp());
        PacketPool::shareInstance().releasePacket(packet);
    }
}

void Link::reportError(ErrorInfo&& error) noexcept {
    if(handler_.expired()){
        loge("link receive data, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler){
        handler->onError(std::move(error));
    }
}

void Link::reportState(ConnectorState state) noexcept {
    if(handler_.expired()){
        loge("link report state, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    assert(handler != nullptr);
    if(handler){
        handler->onStateChanged(state);
    }
}

bool Link::open() noexcept {
    //connector_ = ConnectorManager::shareInstance().newConnector();
    return false;
}

void Link::close() noexcept {
    if(connector_){
        connector_->close();
    }
}

ConnectorState Link::state() const noexcept {
    if(connector_){
        return connector_->state();
    }
    return ConnectorState::Unknown;
}

