//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "File.hpp"
#include <cstdio>

using namespace firefly::FileUtil;


IFile::IFile(std::fstream::openmode openMode, const std::string& path)
    : openMode_(openMode)
    , path_(path)
    , fstream_(path, openMode_)
    , fileMode_(FileMode::None) {
    
}

IFile::IFile(std::fstream::openmode openMode, std::string&& path)
    : openMode_(openMode)
    , path_(path)
    , fstream_(path, openMode_)
    , fileMode_(FileMode::None) {
    
}

IFile::IFile(std::fstream::openmode openMode, const std::string& path, FileMode mode)
    : openMode_(openMode | static_cast<std::fstream::openmode>(mode))
    , path_(path)
    , fstream_(path, openMode_)
    , fileMode_(mode) {
    
}

IFile::IFile(std::fstream::openmode openMode, std::string&& path, FileMode mode)
    : openMode_(openMode | static_cast<std::fstream::openmode>(mode))
    , path_(path)
    , fstream_(path, openMode_)
    , fileMode_(mode) {
    
}

IFile::~IFile() {
    if (fstream_.is_open()) {
        fstream_.close();
    }
}


WriteFile::WriteFile(const std::string& path)
    : IFile(std::fstream::out, path)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
}

WriteFile::WriteFile(std::string&& path)
    : IFile(std::fstream::out, path)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
}

WriteFile::WriteFile(const std::string& path, FileMode mode)
    : IFile(std::fstream::out, path, mode)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
    
}

WriteFile::WriteFile(std::string&& path, FileMode mode)
    : IFile(std::fstream::out, path, mode)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
    
}


WriteFile::~WriteFile() {
    flush();
}


void WriteFile::flush() {
    if (fstream_) {
        fstream_.flush();
        writeCount_ = 0;
    }
}

void WriteFile::write(const std::string& str) {
    write((str.c_str()), str.length());
}

void WriteFile::write(std::string&& str) {
    write((str.c_str()), str.length());
}

void WriteFile::write(const char *data, uint32_t size) {
    if (!fstream_.is_open()) {
        return;
    }
    writeCount_++;
    fstream_.write(data, size);
    writeSize_ += size;
    if (writeCount_ >= checkEveryN_) {
        flush();
    }
}

ReadFile::ReadFile(const std::string& path)
    : IFile(std::fstream::in, path)
    , readSize_(0)
    , readOver_(false) {
}

ReadFile::ReadFile(std::string&& path)
    : IFile(std::fstream::in, path)
    , readSize_(0)
    , readOver_(false) {
    
}

ReadFile::ReadFile(std::string&& path, FileMode mode)
    : IFile(std::fstream::in, path, mode)
    , readSize_(0)
    , readOver_(false) {
    
}

ReadFile::ReadFile(const std::string& path, FileMode mode)
    : IFile(std::fstream::in, path, mode)
    , readSize_(0)
    , readOver_(false) {
    
}

char ReadFile::readCh() {
    if (!fstream_.is_open()) {
        return EOF;
    }
    int ch = fstream_.get();
    if (ch == EOF) {
        readOver_ = true;
    }
    readSize_++;
    return (char) ch;
}

//will discard delim, you can append delim
std::string ReadFile::readUntilChar(char delim) {
    if (fstream_.is_open()) {
        char buffer[kReadBuffSize] = {0};
        fstream_.getline(buffer, kReadBuffSize, delim);
        return {buffer};
    }
    return "";
}

std::string ReadFile::readLine() {
    return readUntilChar('\n');
}

std::string ReadFile::readWord() {
    return readUntilChar(' ');
}

void ReadFile::putback(char ch) {
    if (fstream_.is_open()) {
        fstream_.putback(ch);
    }
}

void ReadFile::unget() {
    if (fstream_.is_open()) {
        fstream_.unget();
    }
}

File::File(const std::string& path)
    : IFile(std::fstream::in | std::fstream::out, path)
    , WriteFile(path)
    , ReadFile(path) {
}

File::File(std::string&& path)
    : IFile(std::fstream::in | std::fstream::out, path)
    , WriteFile(path)
    , ReadFile(path) {
    
}

File::File(const std::string& path, FileMode mode)
    : IFile(std::fstream::in | std::fstream::out, path, mode)
    , WriteFile(path, mode)
    , ReadFile(path, mode) {
    
}

File::File(std::string&& path, FileMode mode)
    : IFile(std::fstream::in | std::fstream::out, path, mode)
    , WriteFile(path, mode)
    , ReadFile(path, mode) {
    
}

File::~File() {
    flush();
}

