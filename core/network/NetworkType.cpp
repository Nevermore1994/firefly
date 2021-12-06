//
// Created by Nevermore on 2021/10/22.
// firefly IPAddress
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "NetworkType.hpp"
#include <cstring>

using namespace firefly::Network;

IPv4 IPAddressInfo::getIPv4() {
    assert(type == IPType::IPv4);
    auto p = std::get_if<IPv4>(&ip);
    if(p){
        return *p;
    }
    return IPv4{};
}

IPv6 IPAddressInfo::getIPv6() {
    assert(type == IPType::IPv6);
    auto p = std::get_if<IPv6>(&ip);
    if(p){
        return *p;
    }
    return IPv6{};
}

SocketAddressInfo::SocketAddressInfo()
        :ipInfo()
        ,port(0)
        ,socketInfo(){
    
}

SocketAddressInfo::SocketAddressInfo(IPAddressInfo info, Port port)
    :ipInfo(info)
    ,port(port){
    if(ipInfo.type == IPType::IPv4){
        SocketAddress address;
        address.sin_addr = ipInfo.getIPv4();
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
#ifdef __APPLE__
        address.sin_len = sizeof(SocketAddress);
#endif
        socketInfo = address;
    } else if(info.type == IPType::IPv6){
        SocketAddressv6 address;
        address.sin6_addr = ipInfo.getIPv6();
        address.sin6_family = AF_INET6;
        address.sin6_flowinfo = 0;
        address.sin6_scope_id = ipInfo.scopeID;
        address.sin6_port = htons(port);
#ifdef __APPLE__
        address.sin6_len = sizeof(SocketAddressv6);
#endif
        socketInfo = address;
    }
}

uint32_t SocketAddressInfo::size() const noexcept {
    if(ipInfo.type == IPType::IPv4){
        return sizeof(SocketAddress);
    } else if(ipInfo.type == IPType::IPv6){
        return sizeof(SocketAddressv6);
    }
    return 0;
}

SocketAddr SocketAddressInfo::addr() const noexcept {
    SocketAddr addr;
    if(ipInfo.type != IPType::Unknown){
        memcpy(&addr, &socketInfo, size());
        return addr;
    }
    assert(ipInfo.type == IPType::Unknown);
    return addr;
}
