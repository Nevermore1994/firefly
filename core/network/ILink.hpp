//
// Created by Nevermore on 2021/11/2.
// firefly ILink
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <cstdint>

namespace firefly
{

class ILink
{
public:
    ILink() = default;

public:
    virtual ~ILink() = default;
    virtual void onConnected() = 0;
    virtual void onError() = 0;
    virtual void onDataReceived(const char* data, uint32_t size, uint64_t receivedTimeStamp) = 0;
    virtual LinkType linkType() = 0;
};

class ILinkHandler
{
public:
    ILinkHandler() = default;
    virtual ~ILinkHandler() = default;
};


} // namespace firefly



