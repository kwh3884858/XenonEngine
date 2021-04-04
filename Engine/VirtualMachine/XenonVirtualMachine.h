//  VirtualMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
namespace VirtualMachine
{

    class XenonVirtualMachine:public CrossPlatform::XenonManager<XenonVirtualMachine>
    {
    public:
        virtual bool Initialize()override;
        virtual bool shutdown()override;
    private:

    };

}
