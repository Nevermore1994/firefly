//
// Created by Nevermore on 2021/10/22.
// firefly FileUtility
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "FileUtility.hpp"
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace firefly;

bool FileUtil::isDirExist(const std::string& path){
    return access(path.c_str(), F_OK) == 0;
}

bool FileUtil::isFileExist(const std::string& path){
    return access(path.c_str(), F_OK) == 0;
}

bool FileUtil::createDir(const std::string& path){
    if (path.empty()){
        return false;
    }
    
    std::string createPath = path;
    if (*(path.rbegin()) != '/'){
        createPath += "/";
    }
    
    std::string subPath;
    for (uint32_t i = 1; i < createPath.size(); ++i){
        if (createPath[i] != '/'){
            continue;
        }
        
        subPath = createPath.substr(0, i);
        if (access(subPath.c_str(), F_OK) == 0){
            continue;
        }
        
        if (mkdir(subPath.c_str(), 0755) == -1){
            return false;
        }
    }
    
    return true;
}

bool FileUtil::deleteFile(const std::string& path){
    return remove(path.c_str()) == 0;
}

bool FileUtil::renameFile(const std::string& oldName, const std::string& newName){
    return rename(oldName.c_str(), newName.c_str()) == 0;
}

int64_t FileUtil::getFileSize(const std::string& path){
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) == 0)
        return statbuf.st_size;
    return -1;
}

bool FileUtil::resizeFile(const std::string& path, uint64_t length){
    return truncate(path.c_str(), length) == 0;
}

bool FileUtil::removeDir(const std::string& path, bool isRetain){
    if (path.empty()){
        return true;
    }
    DIR* dir = nullptr;
    struct dirent* dirinfo = nullptr;
    struct stat statbuf;
    lstat(path.c_str(), &statbuf);
    bool res = true;
    
    if (S_ISREG(statbuf.st_mode)){
        remove(path.c_str());
    } else if (S_ISDIR(statbuf.st_mode)){
        if ((dir = opendir(path.c_str())) == NULL)
            return true;
        while ((dirinfo = readdir(dir)) != NULL){
            std::string filePath = path;
            filePath.append("/");
            filePath.append(dirinfo->d_name);
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)
                continue;
            res = res && removeDir(filePath);
            if (isRetain){
                remove(filePath.c_str());
            }
        }
        closedir(dir);
        if (!isRetain) {
            remove(path.c_str());
        }
    }
    
    return res;
}