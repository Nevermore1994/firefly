//
// Created by Nevermore on 2021/12/7.
// firefly Link
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Link.hpp"
#include "Log.hpp"
#include "PacketPool.hpp"

using namespace firefly::Network;

Link::Link(LinkType type)
    :linkType_(type){
    
}


Link::Link(LinkType type, std::weak_ptr<ILinkHandler> handler)
    :linkType_(type)
    ,handler_(std::move(handler)){
    
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
    if(handler){
        handler->onDataReceived(reinterpret_cast<const char*>(packet->front()), packet->size(), Util::nowTimeStamp());
        PacketPool::shareInstance().releasePacket(packet);
    }
}

void Link::reportError(firefly::ErrorInfo&& error) noexcept {
    if(handler_.expired()){
        loge("link receive data, but handler is expired");
        return;
    }
    auto handler = handler_.lock();
    if(handler){
        handler->onError();
    }
}

void Link::reportState(ConnectorState state) noexcept {

}
