//  Database.h
//  MacWindow
//
//  Created by whkong on 2021-3-31.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/EngineConfig.h"
#include "CrossPlatform/XenonManager.h"
namespace CrossPlatform
{
    class Database final : public XenonManager<Database>{
    public:

        //initilize function, take the place of constructor
        virtual bool Initialize() override;

        //destroy function, take the  place of deconstructor
        virtual bool Shutdown() override { return true; }

        EngineConfig engineConfig;
    };

}