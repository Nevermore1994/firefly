//
// Created by Nevermore on 2021/12/26.
// firefly HttpRequest
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "Packet.hpp"
#include "Time.hpp"
#include "NetworkType.hpp"

namespace firefly::Network{

enum class HttpRequestType{
    Default = 0,
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
};

class HttpRequest{
public:
    explicit HttpRequest(const std::string& url);
    explicit HttpRequest(std::string&& url);
    ~HttpRequest() noexcept = default;
private:
    bool isJoin_; //Has been sent
    HttpRequestType requestType_;
    uint32_t timeoutTime_;	//ms
    Time::TimeStamp timestamp_;
    uint64_t rangeStart_;
    uint64_t rangeEnd_;
    SocketAddressInfo remoteInfo_;
    std::string requestId_;
    std::string requestUrl_;	//start with http:// https://
    std::string host_;
};

struct HttpResponse
{
public:
    HttpResponse() = default;
    ~HttpResponse() noexcept = default;
private:
    uint32_t statusCode = 0;
    uint64_t contentLength = 0;
    std::shared_ptr<Packet> data;
};

class IHttpRequestHandler
{
public:
    virtual void onHttpHeader(std::shared_ptr<HttpRequest> request, const std::string& data) noexcept = 0;
    virtual void onHttpData(std::shared_ptr<HttpRequest> request, const uint8_t* data, uint32_t length) noexcept = 0;
    virtual void onHttpResponse(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response) noexcept = 0;
    virtual ~IHttpRequestHandler() noexcept = default;
};

}




