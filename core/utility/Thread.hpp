//
//  Thread.hpp
//  firefly
//
//  Created by 谭平 on 2021/9/29.
//

#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <functional>

namespace firefly {

class Thread{

public:
    template<class Func, typename ... Args>
    Thread(const std::string&& name, Func&& f, Args&& ... args)
        :name_(name)
        ,isRuning_(false)
        ,isExit_(false)
        ,worker_(&Thread::process, this)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
    
    template<class Func, typename ... Args>
    Thread(const std::string& name, Func&& f, Args&& ... args)
        :name_(name)
        ,isRuning_(false)
        ,isExit_(false)
        ,worker_(&Thread::process, this)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
    
    Thread(const std::string&& name);
    
    Thread(const std::string& name);
    
    ~Thread() noexcept;
    
    void setFunc() noexcept;
    
    void start() noexcept;
    
    void pause() noexcept;
    
    void resume() noexcept;
    
    void stop() noexcept;
    
    template<class Func, typename ... Args>
    void setFunc(Func&& f, Args&& ... args){
        std::unique_lock<std::mutex> lock(mutex_);
        func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    }
public:
    inline bool isRuning() const noexcept {
        return isRuning_.load();
    }
private:
    void process() noexcept;
private:
    std::function<void()> func_;
    std::atomic_bool isRuning_;
    std::atomic_bool isExit_;
    std::string name_;
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

}//end namespace firefly

