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

enum class FileMode{
    append = std::ios_base::app,
    end    = std::ios_base::ate,
    binary = std::ios_base::binary,
    in     = std::ios_base::in,
    out    = std::ios_base::out,
    trunc  = std::ios_base::trunc,
};

class File : public NoCopyable{
public:
    explicit File(const std::string& path, FileMode mode);
    
    explicit File(std::string&& path, FileMode mode);
    
    ~File() override;

public:
    bool open();
    
    void close();
    
    inline std::string_view path() {
        return std::string_view(path_);
    }
    
    inline const std::fstream& stream() const{
        return *file_;
    }

protected:
    std::string path_;
    std::unique_ptr<std::fstream> file_;
    FileMode mode_;
};

}//end namespace firefly::FileUtil
