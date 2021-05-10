//  FileReader.h
//  MacWindow
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/Interface/IFileReader.h"

namespace File
{
    class FileReader : public CrossPlatform::IFileReader
    {
    public:
        virtual XenonFile*const ReadFile(const char* const fileName )const override;
    };
}