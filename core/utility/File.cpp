//
// Created by 谭平 on 2021/10/11.
//

#include "File.hpp"
#include <cstdio>

using namespace firefly;

FileUtil::IFile::IFile(const std::string& path, FileMode mode)
        :path_(path)
        ,mode_(mode){

}

FileUtil::IFile::IFile(std::string&& path, FileMode mode)
        :path_(path)
        ,mode_(mode){

}

FileUtil::IFile::~IFile(){
    if(file_){
        fclose(file_);
        file_ = nullptr;
    }
}

FileUtil::WriteFile::WriteFile(const std::string& path)
        :IFile(path, FileMode::WriteMode)
        ,checkEveryN_(kCheckCount)
        ,writeCount_(0)
        ,writeSize_(0)
        ,checkCount_(0)
{
}

FileUtil::WriteFile::WriteFile(std::string&& path)
        :IFile(path, FileMode::WriteMode)
        ,checkEveryN_(kCheckCount)
        ,writeCount_(0)
        ,writeSize_(0)
        ,checkCount_(0){
}

FileUtil::WriteFile::~WriteFile(){
}

bool FileUtil::WriteFile::open(){
    if(path_.empty())
        return false;
    file_ = fopen(path_.c_str(), ModeStr[(int)mode_]);
    writeSize_ = 0;
    writeCount_ = 0;
    return file_ != nullptr;
}

void FileUtil::WriteFile::close(){
    if(file_){
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

void FileUtil::WriteFile::flush(){
    if(file_){
        fflush(file_);
        writeCount_ = 0;
    }
}

void FileUtil::WriteFile::write(const std::string& str){
    write(str.c_str(), str.length());
}

void FileUtil::WriteFile::write(const char* str, uint32_t size){
    if(!file_){
        return;
    }
    writeCount_ ++;
    size = fwrite(str, 1 , size, file_);
    writeSize_ += size;
    if(writeCount_ >= checkEveryN_){
        flush();
    }
}

FileUtil::ReadFile::ReadFile(const std::string& path)
        :IFile(path, FileMode::ReadMode)
        ,readSize_(0)
        ,readOver_(false)
{
}

FileUtil::ReadFile::ReadFile(std::string&& path)
        :IFile(path, FileMode::ReadMode)
        ,readSize_(0)
        ,readOver_(false)
{

}

FileUtil::ReadFile::~ReadFile()
{

}

char FileUtil::ReadFile::readCh(){
    if(file_) {
        int ch = getc(file_);
        if (ch == EOF) {
            readOver_ = true;
        }
        readSize_++;
        return ch;
    }
    return EOF;
}

void FileUtil::ReadFile::backFillChar(char ch){
    if(file_){
        ungetc(ch, file_);
    }
}

std::string FileUtil::ReadFile::readUntilChar(char ch){
    if(file_){
        std::string res;
        int c = getc(file_);
        while(ch != c && c != EOF){
            res.append(1, ch);
            readSize_ ++;
            ch = getc(file_);
        }
        if(ch == EOF){
            readOver_ = true;
        }
        return res;
    }
    return "";
}

std::string FileUtil::ReadFile::readLine(){
    return readUntilChar('\n');
}

std::string FileUtil::ReadFile::readWord(){
    return readUntilChar(' ');
}

bool FileUtil::ReadFile::open(){
    if(path_.empty())
        return false;
    file_ = fopen(path_.c_str(), ModeStr[(int)mode_]);
    readSize_ = 0;
    readOver_ = false;
    return file_ != nullptr;
}

void FileUtil::ReadFile::close(){

}

FileUtil::File::File(const std::string& path)
        :IFile(path, FileMode::FreeMode)
        ,WriteFile(path)
        ,ReadFile(path)
{
}

FileUtil::File::File(std::string&& path)
        :IFile(path, FileMode::FreeMode)
        ,WriteFile(path)
        ,ReadFile(path)
{

}

FileUtil::File::~File(){

}

bool FileUtil::File::open()
{
    if(path_.empty())
        return false;
    file_ = fopen(path_.c_str(), ModeStr[(int)mode_]);
    return file_ != nullptr;
}

void FileUtil::File::close(){
    if(file_){
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}
