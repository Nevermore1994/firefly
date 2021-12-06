//
// Created by Nevermore on 2021/11/2.
// firefly ILink
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <cstdint>
#include <memory>
#include "NetworkType.hpp"

namespace firefly::Network
{

class ILinkHandler
{
public:
    ILinkHandler() noexcept = default;
    virtual ~ILinkHandler() = default;
};


class ILink
{
public:
    ILink() = default;

public:
    virtual ~ILink() noexcept = default;
    virtual void onConnected() noexcept = 0;
    virtual void onError() noexcept = 0;
    virtual void onDataReceived(const char* data, uint32_t size, uint64_t receivedTimeStamp) noexcept = 0;
    virtual LinkType linkType() noexcept = 0;
    virtual void setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept = 0;
};

} // namespace firefly



