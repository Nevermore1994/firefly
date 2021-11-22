//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "IConnector.hpp"
#include "IPAddress.hpp"
#include "Utility.hpp"
#include "Packet.hpp"
#include "Buffer.hpp"
#include <memory>
#include <list>

namespace firefly::Network{

constexpr uint32_t kReceiveBufferSize = 2 * 1024 * 1024;//2MB
constexpr uint32_t kSendBufferSize = 2 * 1024 * 1024;
constexpr uint32_t kReceiveSize = 1024 * 512; //512kb

using ReceiveBuffer = Buffer<uint8_t, kReceiveBufferSize * 2>;

struct ConnectorInfo{
    bool isBindPort{};
    SocketAddressInfo localIP;
    SocketAddressInfo remoteIP;
    ConnectorType connectorType;
    LinkType linkType;

public:
    ConnectorInfo()
        :isBindPort(false)
        ,localIP()
        ,remoteIP()
        ,connectorType(ConnectorType::ReadWrite)
        ,linkType(LinkType::UDP){
        
    }

public:
    inline bool isTcpLink() const noexcept{
         return linkType == LinkType::TCP;
    }
    
    inline bool isUdpLink() const noexcept {
        return linkType == LinkType::UDP;
    }
    
    inline bool isIPv4() const noexcept{
        return remoteIP.ipInfo.type == IPType::IPv4;
    }
};

class Connector:public IConnector{
public:
    explicit Connector(std::unique_ptr<ConnectorInfo> info);
    ~Connector() = default;

public:
    bool isActive() const noexcept override;
    bool isReadable() const noexcept override;
    bool isWriteable() const noexcept override;
    bool open(IPAddressInfo ip, Port port) noexcept override;
    bool open(SocketAddressInfo ipInfo) noexcept override;
    void close() noexcept override;
    void onReceived() override;
    void onError(ErrorInfo&& info) override;
    void onSend() override;

public:
    void setDelay(bool delay);
    
public:
    inline ConnectorID getID() const noexcept override{
        return id_;
    }
    
    inline ConnectorState state() const noexcept{
        return state_;
    }
    
    inline const ConnectorInfo& getInfo() const noexcept{
        return *info_;
    }
    
    inline bool isDelay() const noexcept{
        return isDelay_;
    }
    
    inline Socket getSocket() const noexcept{
        return socket_;
    }
    
    inline bool isValid() const noexcept{
        return state_ != ConnectorState::Error && state_ != ConnectorState::Disconnected;
    }
private:
    bool initData() noexcept;
    bool open() noexcept;
    bool setSocketConfig(int32_t level, int32_t optName, const char* value, size_t size) const;
    void setState(ConnectorState state) noexcept;
    void setEvent(ConnectorEvent event) noexcept;
    void reportErrorInfo() noexcept;
    void postData() noexcept;
private:
    std::unique_ptr<ConnectorInfo> info_;
    ConnectorState  state_;
    Socket  socket_;
    ConnectorID id_;
    ReceiveBuffer receiveBuffer_;
    std::list<Packet> sendPackets_;
    bool isDelay_;
    std::weak_ptr<IConnectorHandler> manager_;
};

}


