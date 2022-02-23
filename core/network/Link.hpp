//
// Created by Nevermore on 2021/12/7.
// firefly Link
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include "ILink.hpp"
#include "Connector.hpp"

namespace firefly::Network {

class Link : public ILink, IConnectorHandler {
public:
    explicit Link(const LinkInfo& info);
    
    Link(const LinkInfo& info, std::weak_ptr<ILinkHandler> handler);
    
    ~Link() override = default;
    
    inline LinkType linkType() const noexcept override{
        return linkInfo_.linkType;
    }
    
    void setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept override;
    
    //IConnectorHandler
    void reportData(std::shared_ptr<Packet> packet) noexcept override;
    
    void reportError(ErrorInfo&& error) noexcept override;
    
    void reportState(ConnectorState state) noexcept override;
    
    //ILink
    bool open() noexcept override;
    
    bool open(SocketAddressInfo info) noexcept override;
    
    void close() noexcept override;
    
    ConnectorState state() const noexcept override;

private:
    LinkInfo linkInfo_;
    std::weak_ptr<ILinkHandler> handler_;
    std::shared_ptr<Connector> connector_;
};

}



