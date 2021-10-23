//
// Created by Nevermore on 2021/10/22.
// firefly NetUtility
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "NetUtility.hpp"
#include <regex>
#include <arpa/inet.h>
#include <netdb.h>

using namespace firefly;
using namespace firefly::NetWork;
using namespace std::literals;

bool isIP(const std::string& ipstr, IPType type){
    if(ipstr.empty())
        return false;
    if(type == IPType::IPv6){
        struct in6_addr addr{};
        return inet_pton(AF_INET6, ipstr.c_str(), (void *)&addr) == 1;
    } else {
        struct in_addr addr{};
        return inet_pton(AF_INET, ipstr.c_str(), (void *)&addr) == 1;
    }
}

bool NetWork::isIP(const std::string& str) {
    return isIPv6(str) || isIPv4(str);
}

bool NetWork::isIPv6(const std::string& ipstr){
    return ::isIP(ipstr, IPType::IPv6);
}

bool NetWork::isIPv6(std::string&& ipstr){
    return ::isIP(ipstr, IPType::IPv6);
}

bool NetWork::isIPv4(const std::string& ipstr){
    return ::isIP(ipstr, IPType::IPv4);
}

bool NetWork::isIPv4(std::string&& ipstr){
    return ::isIP(ipstr, IPType::IPv4);
}

IPAddressInfo NetWork::str2ip(const std::string& str){
    IPAddressInfo address;
    if(isIPv6(str)){
        struct in6_addr addr{};
        inet_pton(AF_INET6, str.c_str(), (void *)&addr);
        address.ip = addr;
        address.type = IPType::IPv6;
    } else {
        struct in_addr addr{};
        inet_pton(AF_INET, str.c_str(), (void *)&addr);
        address.ip = addr;
        address.type = IPType::IPv4;
    }
    return address;
}

std::string NetWork::ip2str(IPAddressInfo ip){
    char tempRes[128] = {0};
    if(ip.type == IPType::IPv4){
        struct in_addr addr = std::get<IPv4>(ip.ip);
        inet_ntop(AF_INET, &addr,tempRes, sizeof(tempRes));
    } else {
        struct in6_addr addr = std::get<IPv6>(ip.ip);
        inet_ntop(AF_INET6, &addr,tempRes, sizeof(tempRes));
    }
    return {tempRes};
}

bool parseHostAddr(const std::string& host, std::variant<SocketAddress, SocketAddressv6>& ipInfo)
{
    struct addrinfo hints{};
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG|AI_V4MAPPED;
    
    if (isIP(host)) {
        hints.ai_flags |= AI_NUMERICHOST;
    }
    
    constexpr static const char* kService = "http";
    struct addrinfo *result = nullptr;
    auto res = getaddrinfo(host.c_str(), kService, &hints, &result);
    
    if (res == 0) {
        struct addrinfo* info = result;
        std::vector<struct addrinfo*> addrInfos;
        
        while(info) {
            addrInfos.push_back(info);
            info = result->ai_next;
        }
        freeaddrinfo(result);
        
        if(addrInfos.empty()){
            return false;
        }
        
        int size = static_cast<int>(addrInfos.size());
        auto random = Util::random(0, size - 1);
        auto resInfo = addrInfos[random];
        if(resInfo->ai_family == AF_INET){
            struct sockaddr_in6 sa{};
            memcpy(&sa, resInfo -> ai_addr, resInfo -> ai_addrlen);
            ipInfo.emplace<struct sockaddr_in6>(sa);
        } else {
            struct sockaddr_in sa{};
            memcpy(&sa, resInfo -> ai_addr, resInfo -> ai_addrlen);
            ipInfo.emplace<struct sockaddr_in>(sa);
        }
        return true;
    } else {
        gai_strerror(res);
    }
    return false;
}


bool NetWork::parseHost(const std::string& host, IPAddressInfo& ipInfo){
    std::variant<SocketAddress, SocketAddressv6> info;
    auto isSuccess = parseHostAddr(host, info);
    if(!isSuccess){
        return false;
    }
    
    auto p = std::get_if<SocketAddress>(&info);
    if(p != nullptr){
        ipInfo.ip = p->sin_addr;
        ipInfo.type = IPType::IPv4;
    } else {
        auto t = std::get_if<SocketAddressv6>(&info);
        if(t == nullptr){
            return false;
        }
        ipInfo.ip = t->sin6_addr;
        ipInfo.type = IPType::IPv6;
    }
    return true;
}

