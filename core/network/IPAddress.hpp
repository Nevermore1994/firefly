//
// Created by Nevermore on 2021/10/22.
// firefly IPAddress
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <variant>

namespace firefly::NetWork{

enum class IPType{
    IPv4 = 1,
    IPv6,
};

using SocketAddress = struct sockaddr_in;
using SocketAddressv6 = struct sockaddr_in6;
using IPv4 = struct in_addr;
using IPv6 = struct in6_addr;
using IPAddr = std::variant<IPv4, IPv6>;
using Port = uint16_t;

struct IPAddressInfo{
    IPAddr ip;
    IPType type;
};


}



