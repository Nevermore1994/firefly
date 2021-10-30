//
// Created by Nevermore on 2021/10/22.
// firefly NetUtility
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <string>
#include "Utility.hpp"
#include "IPAddress.hpp"

namespace firefly::Network{

bool isIP(const std::string& str);
bool isIPv6(const std::string& ipstr);
bool isIPv6(std::string&& ipstr);

bool isIPv4(const std::string& ipstr);
bool isIPv4(std::string&& ipstr);


IPAddressInfo str2ip(const std::string& str);
std::string ip2str(IPAddressInfo ip);

bool parseHost(const std::string& host, IPAddressInfo& ip);
std::string getHostName();
bool getLocalAddress(IPAddressInfo& ip);
}

