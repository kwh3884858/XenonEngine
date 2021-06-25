//  IFileReader.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once
#include "Algorithms/String.h"

namespace Algorithm
{
    template<typename T>
    class StreamingVector;
}

namespace CrossPlatform {

    struct XenonFile;
       
    class IFileReader
    {
    public:
        virtual ~IFileReader() {}

        virtual XenonFile* const ReadFile(const Algorithm::String fileName)const = 0;
        virtual bool WriteFile(const Algorithm::String fileName, const Algorithm::StreamingVector<char>& stream) const = 0;
        virtual Algorithm::String GetApplicationPath()const = 0;
    };
}