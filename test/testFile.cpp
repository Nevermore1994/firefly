//
// Created by Nevermore on 2021/10/14.
// firefly test/testFile
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "testFile.hpp"
#include <iostream>

using namespace firefly::FileUtil;

void testFile(){
    WriteFile writeFile("test.txt");
    for(int i = 0; i < 1000; i++){
        writeFile.write("hello world!\n");
    }
    
    ReadFile readFile("test.txt");
    for(int i = 0; i < 10; i++){
        std::cout << readFile.readCh() << std::endl;
    }
    
    for(int i = 0; i < 100; i++){
        std::cout << readFile.readWord();
    }
    while(!readFile.readOver()){
        std::cout << "" << readFile.readLine();
    }
    std::cout << std::endl;
}