//
// Created by Nevermore on 2021/12/26.
// firefly HttpRequest
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "HttpRequest.hpp"
#include "Utility.hpp"
using namespace firefly::Network;

HttpRequest::HttpRequest(const std::string& url)
    : isJoin_(false)
    , requestType_(HttpRequestType::GET)
    , timeoutTime_(10000)
    , timestamp_(Time::nowTimeStamp())
    , rangeStart_(0)
    , rangeEnd_(0)
    , requestId_(Util::randomString(20))
    , requestUrl_(url){
    
}

HttpRequest::HttpRequest(std::string&& url)
    : isJoin_(false)
    , requestType_(HttpRequestType::GET)
    , timeoutTime_(10000)
    , timestamp_(Time::nowTimeStamp())
    , rangeStart_(0)
    , rangeEnd_(0)
    , requestId_(Util::randomString(20))
    , requestUrl_(url){
    
}

