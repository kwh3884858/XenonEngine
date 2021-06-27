//  VirtualMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"

namespace Algorithm
{
    template<typename T>
    class StreamingVector;
}

namespace XenonEnigne
{

    const char Op_Char_Left_Bracket = '(';
    const char Op_Char_Right_Bracket = ')';
    const char Op_Char_Slash = '/';
    const char Op_Char_Equal = '=';
    const char Op_Char_Double_Quote = '"';
    const char Op_Char_Single_Quote = '\'';

    enum CompilerState {
        Error,
        StateString,

        StateInstructor,        //key word
        StateIdentity,          //variable

        StateStartBracket,      // <
        StateStopBracket,       // >
        StateSlash,             // /
        StateOp,                // :
        StateDoubleQuote,       // "
        State
    };
    class XenonVirtualMachine : public CrossPlatform::XenonManager<XenonVirtualMachine>
    {

    public:
        virtual bool Initialize()override;
        virtual bool Shutdown()override { return true; }

        void LoadScript(Algorithm::StreamingVector<char>* streamedFile);
        void RunScript();
    private:
        //bool Laxer(const char* const content, Token** const tokens);

    };

}
