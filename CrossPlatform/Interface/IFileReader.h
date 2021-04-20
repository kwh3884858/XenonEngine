//  IFileReader.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once
namespace CrossPlatform {
    class IFileReader
    {
    public:
        virtual ~IFileReader() {}

        virtual bool ReadFile(const char* const fileName, char** file)const = 0;
    };
}