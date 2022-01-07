//
// Created by Nevermore on 2021/10/22.
// firefly IPAddress
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <variant>
#include <cassert>

namespace firefly::Network {

enum class IPType {
    Unknown = 0,
    IPv4 = 1,
    IPv6,
};

enum class LinkType {
    Unknown = 0,
    UDP = 1,
    TCP = 2,
};

constexpr const int32_t kSocketInvalid = -1;

using Socket = int32_t;
using SocketAddress = struct sockaddr_in;
using SocketAddressv6 = struct sockaddr_in6;
using SocketAddr = sockaddr;
using SocketAddrType = std::variant<SocketAddress, SocketAddressv6>;

using IPv4 = struct in_addr;
using IPv6 = struct in6_addr;
using IPAddr = std::variant<IPv4, IPv6>;
using Port = uint16_t;

struct IPAddressInfo {
    IPAddr ip;
    IPType type;
    uint32_t scopeID;
    
    IPAddressInfo()
        : type(IPType::Unknown)
        , scopeID(0) {
        
    }
    
    IPv4 getIPv4();
    
    IPv6 getIPv6();
};

struct SocketAddressInfo {
    IPAddressInfo ipInfo;
    Port port;
    SocketAddrType socketInfo;
    
    SocketAddressInfo();
    
    SocketAddressInfo(IPAddressInfo info, Port port);
    
    uint32_t size() const noexcept;
    
    SocketAddr addr() const noexcept;

public:
    inline bool isValid() const noexcept {
        return ipInfo.type != IPType::Unknown;
    }
};

}



