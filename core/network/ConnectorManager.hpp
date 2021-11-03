//
// Created by Nevermore on 2021/11/3.
// firefly ConnectorManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include "IConnector.hpp"
#include <unordered_map>

namespace firefly::Network{

class ConnectorManager {
public:
    static inline ConnectorManager& shareInstance() {
        static ConnectorManager instance;
        return instance;
    }

public:
    ~ConnectorManager();
    
private:
    ConnectorManager();

private:
};

}



