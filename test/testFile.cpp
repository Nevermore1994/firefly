//
// Created by Nevermore on 2021/10/14.
// firefly testFile
// Copyright (c) 2021 Nevermore All rights reserved.
//
#include "testFile.hpp"
#include <iostream>

using namespace firefly::FileUtil;

void testFile(){
    WriteFile writeFile("test.txt");
    writeFile.open();
    for(int i = 0; i < 1000; i++){
        writeFile.write("hello world!\n");
    }
    writeFile.close();
    
    ReadFile readFile("test.txt");
    readFile.open();
    for(int i = 0; i < 10; i++){
        std::cout << readFile.readCh() << std::endl;
    }
    
    for(int i = 0; i < 100; i++){
        std::cout << readFile.readWord();
    }
    while(!readFile.readOver()){
        std::cout << "" << readFile.readLine();
    }
    readFile.close();
}