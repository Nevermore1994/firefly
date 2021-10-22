//
// Created by Nevermore on 2021/10/22.
// firefly NetEngine
// Copyright (c) 2021 Nevermore All rights reserved.
//

namespace firefly::NetWork {

class NetEngine{

public:
    inline static NetEngine& shareInstance(){
        static NetEngine instance;
        return instance;
    }
    
public:
    NetEngine(const NetEngine&) = delete;
    
    NetEngine& operator=(const NetEngine&) = delete;
    
    NetEngine(NetEngine&&) = delete;
    
    NetEngine& operator=(NetEngine&&) = delete;
    
    ~NetEngine();
private:
    NetEngine();
};

}//end namespace network
