//
// Created by Nevermore on 2021/10/22.
// firefly NetEngine
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <memory>
#include "Connector.hpp"
#include "Thread.hpp"
#include "NetworkType.hpp"
#include "NoCopyable.hpp"

namespace firefly::Network {

constexpr const int kMaxSocketSize = 255;

class NetEngine:public NoCopyable{

public:
    inline static NetEngine& shareInstance(){
        static NetEngine instance;
        return instance;
    }
    
public:
    ~NetEngine();
private:
    NetEngine();
public:
    void add(Socket socket, ConnectorType type) noexcept;
    void remove(Socket socket) noexcept;
    void clear() noexcept;
    void stop() noexcept;
    void setHandler(std::weak_ptr<IConnectorManager> handler) noexcept;
private:
    void process() noexcept;
    void checkAllSocket(const std::vector<Socket>& readSockets, const std::vector<Socket>& writeSockets) noexcept;
    bool checkSocket(Socket socket) noexcept;
    void sendData(Socket socket) noexcept;
    void receiveData(Socket socket) noexcept;
private:
    std::unordered_set<Socket> readSet_;
    std::unordered_set<Socket> writeSet_;
    std::unordered_map<Socket, ConnectorType> sockets_;
    std::weak_ptr<IConnectorManager> connectorHandler_;
    std::mutex mutex_;
    std::atomic<bool> isExit_;
    std::shared_ptr<Thread> worker_;
    uint32_t errorCount_ = 0;
};

}//end namespace network
