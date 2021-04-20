//  VirtualMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
namespace XenonEnigne
{

    class XenonVirtualMachine:public CrossPlatform::XenonManager<XenonVirtualMachine>
    {
    public:
        virtual bool Initialize()override { return true; }
        virtual bool Shutdown()override { return true; }
    private:

    };

}
