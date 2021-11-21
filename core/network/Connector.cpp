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
#include <unistd.h>

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
    close();
}

bool Connector::initData() noexcept {
    bool isTcp = info_->isTcpLink();
    int streamType = isTcp ? SOCK_STREAM : SOCK_DGRAM;
    int ipType = info_->isIPv4() ? AF_INET : AF_INET6;
    socket_ = socket(streamType, ipType, isTcp ? IPPROTO_UDP : IPPROTO_TCP);
    
    //disable reuse addr
    int32_t on = 0;
    auto res = setSocketConfig(SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const char*>(&on), sizeof(on));
    if(!res){
        loge("set socket disable reuse addr, error %s", strerror(errno));
        return false;
    }
    
    //disable SIGPIPE
    on = 1;
    res = setSocketConfig(SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const char*>(&on), sizeof(on));
    if(!res){
        loge("set socket disable no SIGPIPE, error %s", strerror(errno));
        return false;
    }
    
    int32_t flag = fcntl(socket_, F_GETFL);
    flag |= O_NONBLOCK;
    res = fcntl(socket_, F_SETFL, flag) == 0 ;
    if(!res){
        loge("set socket no block error %s", strerror(errno));
        return false;
    }
    
    //tcp default no delay
    if(isTcp){
        setDelay(true);
    }
    state_ = ConnectorState::Init;
    return res;
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

void Connector::onError(ErrorInfo&& info) {
    setState(ConnectorState::Error);
    if(!manager_.expired()){
       auto manager = manager_.lock();
       manager->reportError(socket_, std::move(info));
    }
}

void Connector::onReceived() {
    if(!isValid()){
        return;
    }
    if(state_ == ConnectorState::Connecting){
        setState(ConnectorState::Connected);
        return;
    }
    
    int32_t res = 1;
    char buffer[kReceiveSize];
    uint32_t bufferSize = sizeof(buffer);
    if (info_->isTcpLink()){
        while(res){
            res = recv(socket_, buffer, bufferSize, 0);
            if(res < 0) {
                if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN){
                    //not error, continue next
                } else {
                    reportErrorInfo();
                }
                break; //no block, no data
            } else if(res == 0) {
                //remote socket shut down
            } else {
                receiveBuffer_.append(buffer, (uint32_t)res);
                if(res < bufferSize || receiveBuffer_.size() >= kReceiveBufferSize){
                    break;
                }
            }
        }
    } else if (info_->isUdpLink()) {
        SocketAddr* addr = info_->remoteIP.addr();
        socklen_t addrLength = info_->remoteIP.size();
        while(res){
            res = recvfrom(socket_, buffer, bufferSize, 0, addr, &addrLength);
            if(res < 0){
                reportErrorInfo();
            } else if(res > 0){
                receiveBuffer_.append(buffer, res);
            } else {
                break;
            }
        }
    }
    
}

void Connector::onSend() {
    if(!isValid()){
        return;
    }
}

bool Connector::open(IPAddressInfo ip, Port port) noexcept {
    SocketAddressInfo info(ip, port);
    return open(info);
}

bool Connector::open(SocketAddressInfo ipInfo) noexcept {
    info_->remoteIP = ipInfo;
    bool res = open();
    if(!res){
        setState(ConnectorState::Error);
    }
    return res;
}

bool Connector::open() noexcept {
    if(!info_->remoteIP.isValid()){
        return false;
    }
    bool res = true;
    res = initData();
    
    if(!res){
        reportErrorInfo();
        return false;
    }
    
    //update state
    setState(ConnectorState::Connecting);
    
    //bind address
    if (info_->isBindPort){
        auto adder = info_->localIP.socketInfo;
        socklen_t addrLen = info_->localIP.size();
        res = bind(socket_, reinterpret_cast<sockaddr*>(&adder), addrLen) == 0;
        if(!res){
            reportErrorInfo();
            return false;
        }
    }
    
    //connect
    if(info_->isTcpLink()){
        if (connect(socket_, info_->remoteIP.addr(), info_->remoteIP.size()) == kInvalid){
            if(EINPROGRESS == errno){
                return true;
            }
            reportErrorInfo();
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
    if(res){
        setEvent(ConnectorEvent::Add);
    }
    return res;
}

void Connector::close() noexcept {
    if(socket_ == kSocketInvalid){
        return;
    }
    logi("socket close %d", socket_);
    setState(ConnectorState::Disconnected);
    setEvent(ConnectorEvent::Remove);
    ::close(socket_);
    socket_ = kSocketInvalid;
}

bool Connector::setSocketConfig(int32_t level, int32_t optName, const char *value, size_t size) const {
   return setsockopt(socket_, level, optName, value, size) == 0;
}

void Connector::setState(ConnectorState state) noexcept {
    state_ = state;
    if(!manager_.expired()){
        auto manager = manager_.lock();
        manager->reportState(socket_, state);
    }
}

void Connector::setEvent(ConnectorEvent event) noexcept {
    if(!manager_.expired()){
        auto manager = manager_.lock();
        manager->reportEvent(socket_, event);
    }
}

void Connector::reportErrorInfo() noexcept {
    ErrorInfo errorInfo;
    errorInfo.errorNumber = errno;
    onError(std::move(errorInfo));
}


