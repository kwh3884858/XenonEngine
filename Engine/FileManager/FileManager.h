//  FileManager.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
#include "CrossPlatform/Interface/IFileReader.h"

namespace XenonEnigne {
    class FileManager : public CrossPlatform::XenonManager<FileManager>
    {
    public:
        virtual bool Initialize() override { return true; }
        void SetFileReader(CrossPlatform::IFileReader*const fileReader);

        virtual bool Shutdown() override;
        void ReadFile(const char* const fileName, char** file);
    private:
        CrossPlatform::IFileReader* m_fileReader;
    };
}

