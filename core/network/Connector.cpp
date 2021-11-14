//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Connector.hpp"
#include "Log.hpp"
#include "Type.hpp"
#include "NetEngine.hpp"
#include <cstdint>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

using namespace firefly::Network;

Connector::Connector(std::unique_ptr<ConnectorInfo> info)
    :info_(std::move(info))
    ,state_(ConnectorState::Unknown)
    ,socket_(kSocketInvalid)
    ,id_(Util::shortId())
    ,isDelay_(true){
    if(info_->remoteIP.isValid()){
        open();
    }
}

Connector::~Connector() {

}

void Connector::initData() noexcept {
    bool isTcp = info_->isTcpLink();
    int streamType = isTcp ? SOCK_STREAM : SOCK_DGRAM;
    int ipType = info_->isIPv4() ? AF_INET : AF_INET6;
    socket_ = socket(streamType, ipType, isTcp ? IPPROTO_UDP : IPPROTO_TCP);
    
    //disable reuse addr
    int32_t on = 0;
    auto res = setSocketConfig(SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const char*>(&on), sizeof(on));
    if(!res){
        loge("set socket disable reuse addr, error %s", strerror(errno));
    }
    
    //disable SIGPIPE
    on = 1;
    res = setSocketConfig(SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const char*>(&on), sizeof(on));
    if(!res){
        loge("set socket disable no SIGPIPE, error %s", strerror(errno));
    }
    
    int32_t flag = fcntl(socket_, F_GETFL);
    flag |= O_NONBLOCK;
    res = fcntl(socket_, F_SETFL, flag) == 0 ;
    if(!res){
        loge("set socket no block error %s", strerror(errno));
    }
    
    //tcp default no delay
    if(isTcp){
        setDelay(true);
    }
    state_ = ConnectorState::Init;
}

void Connector::setDelay(bool delay) {
    int32_t flag = delay ? 1 : 0;
    isDelay_ = delay;
    auto res = setSocketConfig(IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&flag), sizeof(flag));
    if(!res){
        loge("set socket delay %d, error %s", flag, strerror(errno));
    }
}

bool Connector::isActive() const noexcept {
    return state_ != ConnectorState::Unknown && state_ != ConnectorState::Disconnected;
}

bool Connector::isReadable() const noexcept {
    auto flag = static_cast<int32_t>(info_->connectorType);
    return (flag & kReadableFlag) > 0;
}

bool Connector::isWriteable() const noexcept {
    auto flag = static_cast<int32_t>(info_->connectorType);
    return (flag & kWriteableFlag) > 0;
}

void Connector::onError() {

}

void Connector::onReceived() {

}

void Connector::onSend() {

}

bool Connector::open(IPAddressInfo ip, Port port) noexcept {
    SocketAddressInfo info(ip, port);
    info_->remoteIP = info;
    open();
}

bool Connector::open(SocketAddressInfo ipInfo) noexcept {
    info_->remoteIP = ipInfo;
    return open();
}

bool Connector::open() noexcept {
    if(!info_->remoteIP.isValid()){
        return false;
    }
    initData();
    
    //bind address
    if (info_->isBindPort){
        auto adder = info_->localIP.socketInfo;
        socklen_t addrLen = info_->localIP.size();
        auto res = bind(socket_, reinterpret_cast<sockaddr*>(&adder), addrLen) == 0;
        if(!res){
            onError();
            return false;
        }
    }
    
    //connect
    bool res = true;
    if(info_->isTcpLink()){
        if (connect(socket_, reinterpret_cast<sockaddr*>(&info_->remoteIP.socketInfo), info_->remoteIP.size()) == kInvalid){
            auto error = errno;
            if(EINPROGRESS == error){
                return true;
            }
            onError();
            res = false;
        }
    } else {
        //set receiveBufferSize
        uint32_t receiveBufferSize = kReceiveBufferSize;
        while (!setSocketConfig(SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&receiveBufferSize), sizeof(receiveBufferSize))){
            receiveBufferSize = kReceiveBufferSize / 2;
            if(receiveBufferSize < kReceiveBufferSize / 4){
                res = false;
                break;
            }
        }
        if(res){
            logi("setting socket %d, receiveBufferSize %d", socket_, receiveBufferSize);
        }
    
        //set sendBufferSize
        uint32_t sendBufferSize = kSendBufferSize;
        while (!setSocketConfig(SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&sendBufferSize), sizeof(sendBufferSize))){
            sendBufferSize = sendBufferSize / 2;
            if(sendBufferSize < sendBufferSize / 4){
                res = false;
                break;
            }
        }
        if(res){
            logi("setting socket %d, receiveBufferSize %d", socket_, sendBufferSize);
        }
    }
    
    //update state
    if(res){
        state_ = ConnectorState::Connecting;
    }
    return res;
}

void Connector::close() noexcept {
    logi("socket close %d", socket_);
    state_ = ConnectorState::Disconnected;
    socket_ = kSocketInvalid;
}

bool Connector::setSocketConfig(int32_t level, int32_t optName, const char *value, size_t size) const {
   return setsockopt(socket_, level, optName, value, size) == 0;
}

