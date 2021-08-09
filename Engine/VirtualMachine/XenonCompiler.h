//  XenonCompiler.h
//  XenonEngine
//
//  Created by whkong on 2021-4-10.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

namespace XenonEngine {
    class XenonScriptAssemblerMachine;
    class XenonVirtualMachine;

    class XenonCompiler
    {
    public:
        XenonCompiler();
        ~XenonCompiler();

        void Initialize();
        void Shutdown();
        void Recompile();
		void RunScript();
    private:
        XenonScriptAssemblerMachine* m_xsam;
        XenonVirtualMachine* m_xvm;
    };


}