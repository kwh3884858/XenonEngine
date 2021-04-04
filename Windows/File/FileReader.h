//  FileReader.h
//  MacWindow
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once


namespace File
{
    class FileReader 
    {
    public:
        void ReadFile(const char* const fileName, char** file);
    };
}