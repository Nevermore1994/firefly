//
// Created by Nevermore on 2021/12/7.
// firefly Link
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "ILink.hpp"
#include "Connector.hpp"

namespace firefly::Network{

class Link :public ILink, IConnectorHandler{
public:
    explicit Link(LinkType type);
    Link(LinkType type, std::weak_ptr<ILinkHandler> handler);
    
    ~Link() override = default;
    inline LinkType linkType() const noexcept override{
        return linkType_;
    }
    void setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept override;
    
    void reportData(std::shared_ptr<Packet> packet) noexcept override;
    void reportError(ErrorInfo &&error) noexcept override;
    void reportState(ConnectorState state) noexcept override;
private:
    LinkType linkType_;
    std::weak_ptr<ILinkHandler> handler_;
    std::shared_ptr<Connector> connector_;
};

}



