//  Database.h
//  MacWindow
//
//  Created by whkong on 2021-3-31.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/EngineConfig.h"
#include "CrossPlatform/ScaffoldSingleton.h"
namespace CrossPlatform
{
    class Database :ScaffoldSingleton<Database>{
    public:
        Database() {
            engineConfig.m_isFullScreen = false;
            engineConfig.m_width = 800;
            engineConfig.m_height = 600;
        }

        virtual bool initialize() override { return true; }

        //destroy function, take the  place of deconstructor
        virtual bool shutdown() override { return true; }

        EngineConfig engineConfig;
    };

    
}