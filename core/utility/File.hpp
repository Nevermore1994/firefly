//
// Created by 谭平 on 2021/10/11.
//

#pragma once
#include <iostream>

namespace firefly::FileUtil{

using FileHandle = FILE*;

constexpr const char* ModeStr[] = {"rb", "wb", "wb+"};

enum class FileMode
{
    ReadMode = 0,
    WriteMode = 1,
    FreeMode = 2
};

class IFile
{
public:
    explicit IFile(const std::string& path, FileMode mode);
    explicit IFile(std::string&& path, FileMode mode);
    IFile& operator=(const IFile&) = delete;
    IFile(const IFile&) = delete;
    IFile(IFile&&) = delete;
    virtual ~IFile();

public:
    virtual bool open() = 0;
    virtual void close() = 0;

protected:
    std::string path_;
    FileHandle file_;
    FileMode mode_;
};

class WriteFile:virtual public IFile
{
    constexpr static uint32_t kCheckCount = 512;
public:
    explicit WriteFile(const std::string& path);
    explicit WriteFile(std::string&& path);
    virtual ~WriteFile();
    void write(const std::string& str);
    void write(const uint8_t* data, uint32_t size);
    void flush();

public:
    bool open() override;
    void close() override;

protected:
    int32_t checkEveryN_;
    uint32_t checkCount_;
    uint64_t writeSize_;
    uint32_t writeCount_;
};

class ReadFile:virtual public IFile
{
public:
    explicit ReadFile(std::string&& path);
    explicit ReadFile(const std::string& path);
    virtual ~ReadFile();
    std::string readLine();
    char readCh();
    void backFillChar(char ch);
    std::string readWord();
    inline bool readOver() const { return readOver_;}
    inline uint64_t readSize() const { return readSize_; }

public:
    bool open() override;
    void close() override;
    std::string readUntilChar(char ch);

protected:
    bool readOver_;
    uint64_t readSize_;
};

class File final:public ReadFile, public WriteFile
{
public:
    File(const std::string& path);
    File(std::string&& path);
    virtual ~File();

public:
    bool open() final;
    void close() final;
};



}//end namespace firefly::FileUtil