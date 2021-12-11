//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//
#pragma once
#include <iostream>
#include <fstream>
#include "NoCopyable.hpp"

namespace firefly::FileUtil{

enum class FileMode {
    None = 0,
    Append = std::fstream::app,
    Binary = std::fstream::binary,
    Trunc = std::fstream::trunc,
};

class IFile : public NoCopyable {
public:
    IFile(std::fstream::openmode openMode, const std::string& path);
    IFile(std::fstream::openmode openMode, std::string&& path);
    IFile(std::fstream::openmode openMode, const std::string& path, FileMode mode);
    IFile(std::fstream::openmode openMode, std::string&& path, FileMode mode);
    virtual ~IFile();
    
    inline FileMode fileMode() const noexcept {
        return fileMode_;
    }
    
    inline bool isReadFile() const noexcept {
        return (openMode_ & std::fstream::in) == std::fstream::in;
    }
    
    inline bool isWriteFile() const noexcept {
        return (openMode_ & std::fstream::out) == std::fstream::out;
    }
    
    inline std::string_view path() const noexcept {
        return path_;
    }

protected:
    std::fstream::openmode openMode_;
    FileMode fileMode_;
    std::fstream fstream_;
    std::string path_;
};

class WriteFile : virtual public IFile {
    constexpr static uint32_t kCheckCount = 512;
public:
    explicit WriteFile(const std::string& path);
    explicit WriteFile(std::string&& path);
    WriteFile(const std::string& path, FileMode mode);
    WriteFile(std::string&& path, FileMode mode);
    ~WriteFile() override;
    void write(const std::string& str);
    void write(std::string&& str);
    void write(const char *data, uint32_t size);
    void flush();
protected:
    int32_t checkEveryN_;
    uint32_t checkCount_;
    uint64_t writeSize_;
    uint32_t writeCount_;
};

class ReadFile : virtual public IFile {
    constexpr static uint32_t kReadBuffSize = 512;
public:
    explicit ReadFile(std::string&& path);
    explicit ReadFile(const std::string& path);
    ReadFile(std::string&& path, FileMode mode);
    ReadFile(const std::string& path, FileMode mode);
    ~ReadFile() override = default;
    std::string readLine();
    char readCh();
    void unget();
    void putback(char ch);
    std::string readWord();
    inline bool readOver() const noexcept { return !fstream_.is_open() || fstream_.eof(); }
    
    inline uint64_t readSize() const noexcept { return readSize_; }

public:
    std::string readUntilChar(char ch);

protected:
    uint64_t readSize_;
};

class File final : public ReadFile, public WriteFile {
public:
    explicit File(const std::string& path);
    explicit File(std::string&& path);
    File(const std::string& path, FileMode mode);
    File(std::string&& path, FileMode mode);
    ~File() override;
};

}//end namespace firefly::FileUtil