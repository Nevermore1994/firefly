//
//  NetEngine.hpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//

namespace firefly {

namespace network{

class NetEngine{

public:
    inline static NetEngine& shareInstance(){
        static NetEngine instance;
        return instance;
    }
    
public:
    NetEngine(const NetEngine&) = delete;
    
    NetEngine& operator=(const NetEngine&) = delete;
    
    NetEngine(const NetEngine&&) = delete;
    
    NetEngine& operator=(const NetEngine&&) = delete;
    
    ~NetEngine();
private:
    NetEngine();
};

}//end namespace network

}//end namespace firefly
