//  FileManager.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Algorithms/String.h"
//#include "Algorithms/StreamingVector.h"

namespace CrossPlatform {
    class IFileReader;
    struct XenonFile;
}

namespace Algorithm
{
    template<typename T>
    class StreamingVector;

    template<typename T>
    class StringBase;
    typedef StringBase<char> String;
}

namespace XenonEnigne {
    class FileManager : public CrossPlatform::XenonManager<FileManager>
    {
    public:
        virtual bool Initialize() override { return true; }
        void SetFileReader(CrossPlatform::IFileReader*const fileReader);

        virtual bool Shutdown() override;
        CrossPlatform::XenonFile*const ReadFile(const Algorithm::String& fileName)const;
        Algorithm::StreamingVector<char>* ReadStreamFile(const Algorithm::String& fileName)const;
        bool WriteFile(const Algorithm::String& fileName, const Algorithm::StreamingVector<char>& stream)const;
        //CrossPlatform::XenonFile*const ReadFile(const char* const fileName);
        Algorithm::String GetApplicationPath()const;
    private:
        CrossPlatform::IFileReader* m_fileReader;
    };
}

