//  IFileReader.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once
namespace CrossPlatform {

    struct XenonFile;

    class IFileReader
    {
    public:
        virtual ~IFileReader() {}

        virtual XenonFile* const ReadFile(const char* const fileName)const = 0;
        virtual Algorithm::String GetApplicationPath()const = 0;
    };
}