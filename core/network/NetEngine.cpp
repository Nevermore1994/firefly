//
// Created by Nevermore on 2021/10/22.
// firefly NetEngine
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "NetEngine.hpp"
#include "Log.hpp"
#include "ThreadManager.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/select.h>

using namespace firefly;
using namespace firefly::Network;

NetEngine::NetEngine()
    :isExit_(false)
    ,worker_(std::make_shared<Thread>("NetEngine", &NetEngine::process, this)){
    ThreadManager::shareInstance().add(worker_);
}

NetEngine::~NetEngine(){
    stop();
}

void NetEngine::stop() noexcept {
    if(isExit_){
        return;
    }
    isExit_ = true;
    worker_->stop();
    worker_.reset();
}

void NetEngine::process() noexcept {
    while(!isExit_){
        if(readSet_.empty() && writeSet_.empty()){
            usleep(10 * 1000);
            continue;
        }
        
        std::vector<Socket> readAllSockets;
        std::vector<Socket> writeAllSockets;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            readAllSockets.assign(readSet_.begin(), readSet_.end());
            writeAllSockets.assign(writeSet_.begin(), writeSet_.end());
        }
    
        auto readSockets = Util::reservoirSampling(readAllSockets, kMaxSocketSize);
        auto writSockets = Util::reservoirSampling(writeAllSockets, kMaxSocketSize);
        
        fd_set  fdSetRead;
        fd_set  fdSetWrite;
        FD_ZERO(&fdSetRead);
        FD_ZERO(&fdSetWrite);
        Socket maxDescriptor = 0;
        
        for(auto socket : readSockets){
            FD_SET(socket, &fdSetRead);
            maxDescriptor = std::max(socket, maxDescriptor);
        }
        for(auto socket : writSockets){
            FD_SET(socket, &fdSetWrite);
            maxDescriptor = std::max(socket, maxDescriptor);
        }
    
        struct timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 50 * 1000;
        auto res = select(maxDescriptor + 1, &fdSetRead, &fdSetWrite, nullptr, &timeout);
        if(res < 0){
            int errorType = errno;
            ++errorCount_;
            if(errorCount_ >= 50){
                loge("select error:%s", strerror(errno));
                errorCount_ = 0;
            }
            //The argument s is an invalid descriptor
            if(errorType == EBADF){
                checkAllSocket(readAllSockets, writeAllSockets);
            }
        } else if(res > 0) {
            for(auto socket : readSockets){
                if(FD_ISSET(socket, &fdSetRead)){
                    receiveData(socket);
                }
            }
            for(auto socket : writSockets){
                if(FD_ISSET(socket, &fdSetWrite)){
                    sendData(socket);
                }
            }
        }
        //todo
    }
}

void NetEngine::add(Socket socket, std::shared_ptr<Connector> connector) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connector->isReadable()){
        readSet_.insert(socket);
    }
    
    if(connector->isWriteable()){
        writeSet_.insert(socket);
    }
    connectors_.insert(std::make_pair(socket, std::move(connector)));
}

void NetEngine::remove(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket) == 0){
        return;
    }
    auto connector = connectors_.at(socket);
    if(connector->isReadable()){
        readSet_.erase(socket);
    }
    
    if(connector->isWriteable()){
        writeSet_.erase(socket);
    }
    connectors_.erase(socket);
}

void NetEngine::clear() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    readSet_.clear();
    writeSet_.clear();
    connectors_.clear();
}

void NetEngine::sendData(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket) == 0){
        return;
    }
    auto connector = connectors_.at(socket);
    connector->onSend();
}

void NetEngine::receiveData(Socket socket) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    if(connectors_.count(socket) == 0){
        return;
    }
    auto connector = connectors_.at(socket);
    connector->onReceived();
}

void NetEngine::checkAllSocket(const std::vector<Socket>& readSockets, const std::vector<Socket>& writeSockets) noexcept {
    std::vector<Socket> invalidSockets;
    for(auto socket : readSockets){
        if(!checkSocket(socket)){
            invalidSockets.push_back(socket);
        }
    }
    for(auto socket: writeSockets) {
        if(!checkSocket(socket)){
            invalidSockets.push_back(socket);
        }
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        for(auto socket : invalidSockets){
            readSet_.erase(socket);
            writeSet_.erase(socket);
            auto it = connectors_.find(socket);
            if(it != connectors_.end()){
                auto connector = it->second;
                connectors_.erase(socket);
                ErrorInfo errorInfo;
                errorInfo.errorNumber = EBADF;
                connector->onError(std::move(errorInfo));
            }
        }
    }
}

bool NetEngine::checkSocket(Socket socket) noexcept {
    std::shared_ptr<Connector> connector;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if(connectors_.count(socket) == 0){
            return false;
        }
        connector = connectors_.at(socket);
    }
    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(socket, &fdSet);
    
    struct timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    auto res = select(socket + 1, connector->isReadable() ? &fdSet : nullptr, connector->isWriteable() ? &fdSet : nullptr,
                      nullptr, &timeout);
    if(res > 0 || errno != EBADF){
        return true;
    }
    return false;
}

