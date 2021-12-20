//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "Connector.hpp"
#include "Log.hpp"
#include "Type.hpp"
#include "NetEngine.hpp"
#include "PacketPool.hpp"
#include "NetworkConfig.hpp"
#include <cstdint>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>

#ifndef __APPLE__
#include <signal.h>
#endif

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

Connector::Connector(std::unique_ptr<ConnectorInfo> info, std::weak_ptr<IConnectorHandler> handler)
    :info_(std::move(info))
    ,state_(ConnectorState::Unknown)
    ,socket_(kSocketInvalid)
    ,id_(Util::shortId())
    ,isDelay_(true)
    ,handler_(std::move(handler)){
    if(info_->remoteIP.isValid()){
        open();
    }
}


bool Connector::initData() noexcept {
    bool isTcp = info_->isTcpLink();
    int streamType = isTcp ? SOCK_STREAM : SOCK_DGRAM;
    int ipType = info_->isIPv4() ? AF_INET : AF_INET6;
    socket_ = socket(streamType, ipType, isTcp ? IPPROTO_UDP : IPPROTO_TCP);
    
    //disable reuse addr
    int32_t on = 0;
    auto res = setSocketConfig(SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&on), sizeof(on));
    if(!res){
        loge("set socket disable reuse addr, error %s", strerror(errno));
        reportErrorInfo();
        return false;
    }
    
    //disable SIGPIPE
    on = 1;
#ifdef __APPLE__
    res = setSocketConfig(SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const char*>(&on), sizeof(on));
#else
    signal(SIGPIPE, SIG_IGN);
    res = 1;
#endif
    if(!res){
        loge("set socket disable no SIGPIPE, error %s", strerror(errno));
        reportErrorInfo();
        return false;
    }
    
    int32_t flag = fcntl(socket_, F_GETFL);
    flag |= O_NONBLOCK;
    res = fcntl(socket_, F_SETFL, flag) == 0 ;
    if(!res){
        loge("set socket no block error %s", strerror(errno));
        reportErrorInfo();
        return false;
    }
    
    //tcp default no delay
    if(isTcp){
        setDelay(true);
    }
    state_ = ConnectorState::Init;
    logi("create socket success, id %d", socket_);
    return res;
}

void Connector::setDelay(bool delay) {
    auto flag = delay ? 1 : 0;
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
    return isConnectorReadable(info_->connectorType);
}

bool Connector::isWriteable() const noexcept {
    return isConnectorWriteable(info_->connectorType);
}

void Connector::onError(ErrorInfo&& info) noexcept {
    setState(ConnectorState::Error);
    if(!handler_.expired()){
       auto handler = handler_.lock();
        handler->reportError(std::move(info));
    }
}

void Connector::onReceived() noexcept {
    if(!isValid()){
        return;
    }
    if(state_ == ConnectorState::Connecting){
        connected();
        return;
    }
    
    if (info_->isTcpLink()){
        receiveTcpData();
    } else if (info_->isUdpLink()) {
        receiveUdpData();
    }
    postData();
}

void Connector::onSend() noexcept {
    if(!isValid()){
        //remove invalid socket
        setEvent(ConnectorEvent::Remove);
        return;
    }
    if(state_ == ConnectorState::Connecting){
        setState(ConnectorState::Connected);
        return;
    }
    
    //post packet to send
    auto addr = info_->remoteIP.addr();
    {
        auto res = 0l;
        std::unique_lock<std::mutex> lock(mutex_);
        while(!sendPackets_.empty()){
            if(!isValid()){
                break;
            }
            auto packet = sendPackets_.front();
            if(packet->isValid()){
                if(info_->isTcpLink()){
                    res = send(socket_, packet->front(), packet->size(), 0);
                } else if(info_->isUdpLink()){
                    res = sendto(socket_, packet->front(), 0, packet->size(), &addr, info_->remoteIP.size());
                }
#if NETWORK_LOG
                logi("%s socket %d, send data:%d ", info_->isTcpLink() ? "tcp" : "udp", res);
#endif
            }
            
            if(res > 0){
                sendSize_ += res;
            }
            
            if(!packet->isValid() || res >= packet->size()){
                sendPackets_.pop_front();
                PacketPool::shareInstance().releasePacket(packet);
            } else if(res > 0){
                packet->freed(res); //move packet position
            } else if(res < 0 && !isIgnoredError()){
                loge("%s socket %d, send error:%s ", info_->isTcpLink() ? "tcp" : "udp", strerror(errno));
                reportErrorInfo();
                break;
            }
        }
    }
}

bool Connector::open(IPAddressInfo ip, Port port) noexcept {
    SocketAddressInfo info(ip, port);
    return open(info);
}

bool Connector::open(SocketAddressInfo ipInfo) noexcept {
    info_->remoteIP = ipInfo;
    auto res = open();
    if(!res){
        setState(ConnectorState::Error);
    }
    return res;
}

bool Connector::open() noexcept {
    if(!info_->remoteIP.isValid()){
        return false;
    }
    auto res = true;
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
        auto addr = info_->remoteIP.addr();
        if (connect(socket_, &addr, info_->remoteIP.size()) == kInvalid){
            if(EINPROGRESS == errno){
                res = true;
            } else {
                reportErrorInfo();
                res = false;
            }
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

void Connector::connected() noexcept {
    setState(ConnectorState::Connected);
#if NETWORK_LOG
    logi("%s socket:%d connected.", info_->isTcpLink() ? "tcp": "udp", socket_);
#endif
}

bool Connector::setSocketConfig(int32_t level, int32_t optName, const char *value, size_t size) const {
   return setsockopt(socket_, level, optName, value, size) == 0;
}

void Connector::setState(ConnectorState state) noexcept {
    state_ = state;
    if(!handler_.expired()){
        auto handler = handler_.lock();
        handler->reportState(state);
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

void Connector::postData() noexcept {
    //post data
    if(manager_.expired()){
        loge("connector manger is invalid.");
        return;
    }
    if(!receiveBuffer_.empty()){
        auto length = receiveBuffer_.length();
        if(length <= 4){
            //data length is short
            return;
        }
        
        auto packet = PacketPool::shareInstance().newPacket(receiveBuffer_.front(), length);
        auto handler = handler_.lock();
        if(handler){
            handler->reportData(packet);
            receiveBuffer_.reset();
        }
    }
}

bool Connector::receiveTcpData() noexcept {
    auto res = 1l;
    uint8_t buffer[kReceiveSize];
    auto bufferSize = sizeof(buffer);
    auto receiveSize = 0ul;
    while(res){
        res = recv(socket_, buffer, bufferSize, 0);
        if(res < 0) {
            if(isIgnoredError()){
                //not error, continue next
            } else {
                loge("tcp receive, error: %s", strerror(errno));
                reportErrorInfo();
            }
        } else if(res > 0){
            logi("tcp socket %d, receive data len %d", socket_, res);
            receiveBuffer_.append(buffer, res);
            receiveSize += res;
            if(res < bufferSize || receiveBuffer_.length() >= kReceiveBufferSize){
                break;
            }
        }
        //if res <= 0, will be exited this loop
    }
    receiveSize_ += receiveSize;
#if NETWORK_LOG
    logi("receiveTcpData %lu", receiveSize);
#endif
    return receiveSize == 0;
}

bool Connector::receiveUdpData() noexcept {
    auto addr = info_->remoteIP.addr();
    auto addrLength = info_->remoteIP.size();
    auto res = 1l;
    auto receiveSize = 0ul;
    uint8_t buffer[kReceiveSize];
    auto bufferSize = sizeof(buffer);
    while(res){
        res = recvfrom(socket_, buffer, bufferSize, 0, &addr, &addrLength);
        if(res < 0){
            loge("udp receive, error: %s", strerror(errno));
            reportErrorInfo();
        } else if(res > 0){
            logi("udp socket %d, receive data len %d", socket_, res);
            receiveSize += res;
            receiveBuffer_.append(buffer, res);
            if(res < bufferSize || receiveBuffer_.length() >= kReceiveBufferSize){
                break;
            }
        }
        //if res <= 0, will be exited this loop
    }
    receiveSize_ += receiveSize;
#if NETWORK_LOG
    logi("receiveUdpData %lu", receiveSize);
#endif
    return receiveSize == 0;
}

bool Connector::isIgnoredError() noexcept {
    return errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN || errno == ENOBUFS;
}



