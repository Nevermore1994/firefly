//
// Created by Nevermore on 2021/12/6.
// firefly TcpLink
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "ILink.hpp"
#include <memory>

namespace firefly::Network{

class TcpLink:public ILink{

public:
    void onConnected() noexcept override;
    void onError() noexcept override;
    void onDataReceived(const char *data, uint32_t size, uint64_t receivedTimeStamp) noexcept override;
    void setDataHandler(std::weak_ptr<ILinkHandler> handler) noexcept override;
private:
    std::weak_ptr<ILinkHandler> handler_;
};

}


