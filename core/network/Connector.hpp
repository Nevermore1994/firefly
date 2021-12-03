//
// Created by Nevermore on 2021/11/3.
// firefly Connector
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "ConnectorType.hpp"
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

using ReceiveBuffer = Buffer<uint8_t>;

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
    [[nodiscard]] inline bool isTcpLink() const noexcept{
         return linkType == LinkType::TCP;
    }
    
    [[nodiscard]] inline bool isUdpLink() const noexcept {
        return linkType == LinkType::UDP;
    }
    
    [[nodiscard]] inline bool isIPv4() const noexcept{
        return remoteIP.ipInfo.type == IPType::IPv4;
    }
};

class Connector{
public:
    explicit Connector(std::unique_ptr<ConnectorInfo> info);
    ~Connector() = default;

public:
    bool isActive() const noexcept;
    bool isReadable() const noexcept;
    bool isWriteable() const noexcept;
    bool open(IPAddressInfo ip, Port port) noexcept;
    bool open(SocketAddressInfo ipInfo) noexcept;
    void close() noexcept;
    void onReceived() noexcept;
    void onError(ErrorInfo&& info) noexcept;
    void onSend() noexcept;
    void connected() noexcept;
public:
    void setDelay(bool delay);
    
public:
    [[maybe_unused]]
    inline ConnectorID getID() const noexcept{
        return id_;
    }
    
    [[maybe_unused]]
    inline ConnectorState state() const noexcept{
        return state_;
    }
    
    [[maybe_unused]]
    inline const ConnectorInfo& getInfo() const noexcept{
        return *info_;
    }
    
    [[maybe_unused]]
    inline bool isDelay() const noexcept{
        return isDelay_;
    }
    
    [[maybe_unused]]
    inline Socket getSocket() const noexcept{
        return socket_;
    }
    
    [[maybe_unused]]
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
    bool receiveTcpData() noexcept;
    bool receiveUdpData() noexcept;
    static bool isIgnoredError() noexcept;
private:
    std::unique_ptr<ConnectorInfo> info_;
    ConnectorState  state_;
    Socket  socket_;
    ConnectorID id_;
    ReceiveBuffer receiveBuffer_;
    std::list<std::shared_ptr<Packet>> sendPackets_;
    bool isDelay_;
    std::mutex mutex_;
    std::weak_ptr<IConnectorHandler> manager_;
    
    //info
private:
    uint64_t receiveSize_ = 0ull;
    uint64_t sendSize_ = 0ull;
};

}


