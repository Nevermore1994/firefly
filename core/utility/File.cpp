//
// Created by Nevermore on 2021/10/22.
// firefly DnsParserManager
// Copyright (c) 2021 Nevermore All rights reserved.
//

#include "File.hpp"
#include <cstdio>

using namespace firefly::FileUtil;

IFile::IFile(const std::string& path, FileMode mode)
    : path_(path)
    , mode_(mode) {
    
}

IFile::IFile(std::string&& path, FileMode mode)
    : path_(path)
    , mode_(mode) {
    
}

IFile::~IFile() {
    if(file_) {
        fclose(file_);
        file_ = nullptr;
    }
}

WriteFile::WriteFile(const std::string& path)
    : IFile(path, FileMode::WriteMode)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
}

WriteFile::WriteFile(std::string&& path)
    : IFile(path, FileMode::WriteMode)
    , checkEveryN_(kCheckCount)
    , writeCount_(0)
    , writeSize_(0)
    , checkCount_(0) {
}

WriteFile::~WriteFile() {
    flush();
}

bool WriteFile::open() {
    if(path_.empty()) {
        return false;
    }
    if(file_) {
        return true;
    }
    file_ = fopen(path_.c_str(), ModeStr[(int) mode_]);
    writeSize_ = 0;
    writeCount_ = 0;
    return file_ != nullptr;
}

void WriteFile::close() {
    if(file_) {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
        writeSize_ = 0;
        writeCount_ = 0;
    }
}

void WriteFile::flush() {
    if(file_) {
        fflush(file_);
        writeCount_ = 0;
    }
}

void WriteFile::write(const std::string& str) {
    write(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
}

void WriteFile::write(std::string&& str) {
    write(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
}

void WriteFile::write(const uint8_t *data, uint32_t size) {
    if(!file_) {
        return;
    }
    writeCount_++;
    size = fwrite(data, size, 1, file_);
    writeSize_ += size;
    if(writeCount_ >= checkEveryN_) {
        flush();
    }
}

ReadFile::ReadFile(const std::string& path)
    : IFile(path, FileMode::ReadMode)
    , readSize_(0)
    , readOver_(false) {
}

ReadFile::ReadFile(std::string&& path)
    : IFile(path, FileMode::ReadMode)
    , readSize_(0)
    , readOver_(false) {
    
}

ReadFile::~ReadFile() {

}

char ReadFile::readCh() {
    if(file_) {
        int ch = getc(file_);
        if(ch == EOF) {
            readOver_ = true;
        }
        readSize_++;
        return ch;
    }
    return EOF;
}

void ReadFile::backFillChar(char ch) {
    if(file_) {
        ungetc(ch, file_);
    }
}

std::string ReadFile::readUntilChar(char ch) {
    if(file_) {
        std::string res;
        int c = getc(file_);
        while(ch != c && c != EOF) {
            res.append(1, c);
            readSize_++;
            c = getc(file_);
        }
        if(c == EOF) {
            readOver_ = true;
        }
        return res;
    }
    return "";
}

std::string ReadFile::readLine() {
    return readUntilChar('\n');
}

std::string ReadFile::readWord() {
    return readUntilChar(' ');
}

bool ReadFile::open() {
    if(path_.empty()) {
        return false;
    }
    if(file_) {
        return true;
    }
    file_ = fopen(path_.c_str(), ModeStr[(int) mode_]);
    readSize_ = 0;
    readOver_ = false;
    return file_ != nullptr;
}

void ReadFile::close() {
    if(file_) {
        fclose(file_);
        file_ = nullptr;
    }
}

File::File(const std::string& path)
    : IFile(path, FileMode::FreeMode)
    , WriteFile(path)
    , ReadFile(path) {
}

File::File(std::string&& path)
    : IFile(path, FileMode::FreeMode)
    , WriteFile(path)
    , ReadFile(path) {
    
}

File::~File() {
    flush();
}

bool File::open() {
    if(path_.empty()) {
        return false;
    }
    if(file_) {
        return true;
    }
    file_ = fopen(path_.c_str(), ModeStr[(int) mode_]);
    return file_ != nullptr;
}

void File::close() {
    if(file_) {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}
