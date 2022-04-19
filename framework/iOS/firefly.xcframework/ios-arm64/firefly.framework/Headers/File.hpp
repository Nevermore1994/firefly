//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once

#include <fstream>
#include <memory>
#include "NoCopyable.hpp"

namespace firefly::FileUtil {

class File : public NoCopyable{
public:
    explicit File(const std::string& path, std::ios_base::openmode mode);
    
    explicit File(std::string&& path, std::ios_base::openmode mode);
    
    ~File() override;

public:
    bool open();
    
    void close();
    
    inline std::string_view path() {
        return std::string_view(path_);
    }
    
    inline std::fstream& stream(){
        return *file_;
    }
    
    inline std::ios_base::openmode FileMode() const{
        return mode_;
    }

protected:
    std::string path_;
    std::unique_ptr<std::fstream> file_;
    std::ios_base::openmode mode_;
};

}//end namespace firefly::FileUtil
