//
// Created by Nevermore on 2021/11/2.
// firefly ILink
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <cstdint>
#include <memory>
#include "NetworkType.hpp"
#include "Type.hpp"
#include "ConnectorType.hpp"

namespace firefly::Network
{

class ILinkHandler
{
public:
    virtual ~ILinkHandler() = default;
    virtual void onStateChanged(ConnectorState state) noexcept = 0;
    virtual void onError(ErrorInfo&& info) noexcept = 0;
    virtual void onDataReceived(const char* data, uint32_t size, uint64_t receivedTimeStamp) noexcept = 0;
};


class ILink
{
public:
    virtual ~ILink() noexcept = default;
    virtual LinkType linkType() const noexcept = 0;
    virtual bool open() noexcept = 0;
    virtual void close() noexcept = 0;
    virtual ConnectorState state () const noexcept = 0;
    virtual void setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept = 0;
};

struct LinkInfo{
    LinkType linkType = LinkType::Unknown;
    SocketAddressInfo remoteAdderInfo;
};

} // namespace firefly



