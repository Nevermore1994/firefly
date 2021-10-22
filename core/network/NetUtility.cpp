//
// Created by Nevermore on 2021/10/22.
// firefly NetUtility
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "NetUtility.hpp"
#include <regex>
#include <arpa/inet.h>

using namespace firefly;
using namespace firefly::NetWork;
using namespace std::literals;

bool isIP(const std::string& ipstr, IPType type){
    struct in_addr addr;
    int t = type == IPType::IPv6 ? AF_INET : AF_INET6;
    if(ipstr.empty())
        return false;
    if(inet_pton(t, ipstr.c_str(), (void *)&addr) == 1)
        return true;
    return false;
}

bool NetWork::isIPv6(const std::string& ipstr){
    return isIP(ipstr, IPType::IPv6);
}

bool NetWork::isIPv6(std::string&& ipstr){
    return isIP(ipstr, IPType::IPv6);
}

bool NetWork::isIPv4(const std::string& ipstr){
    return isIP(ipstr, IPType::IPv4);
}

bool NetWork::isIPv4(std::string&& ipstr){
    return isIP(ipstr, IPType::IPv4);
}

IPAddressInfo NetWork::str2ip(const std::string& str){
    IPAddressInfo address;
    if(isIPv6(str)){
        struct in6_addr addr;
        inet_pton(AF_INET6, str.c_str(), (void *)&addr);
        address.ip = addr;
        address.type = IPType::IPv6;
    } else {
        struct in_addr addr;
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
    return std::string(tempRes);
}

bool NetWork::parseHost(const std::string& host, IPAddressInfo& ip){

}
