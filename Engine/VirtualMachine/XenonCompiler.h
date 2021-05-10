//  XenonCompiler.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

namespace XenonEnigne {
    class XenonScriptAssemblerMachine;

    class XenonCompiler
    {
    public:
        XenonCompiler();
        ~XenonCompiler();

        void Initialize();

    private:
        XenonScriptAssemblerMachine* m_xsam;

    };


}