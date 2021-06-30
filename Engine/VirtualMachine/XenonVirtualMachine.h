//  VirtualMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Engine/VirtualMachine/XenonStruction.h"
#include "Algorithms/String.h"

namespace Algorithm
{
    template<typename T>
    class StreamingVector;
}

namespace XenonEnigne
{
    using Algorithm::String;
/*
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
*/
    class XenonVirtualMachine : public CrossPlatform::XenonManager<XenonVirtualMachine>
    {
        struct Value 
        {
            union
            {
                int m_interalValue;
                float m_floatValue;
            };
        };
    public:
        virtual bool Initialize()override;
        virtual bool Shutdown()override { return true; }

        bool LoadScript(Algorithm::StreamingVector<char>* streamedFile);
        void RunScript();
    private:
        //bool Laxer(const char* const content, Token** const tokens);
        ScriptHeader m_scriptHeader;
        Vector<Instruction*> m_instructionList;
        Vector<FunctionElement*> m_functionTable;
        Vector<LabelElement*> m_labelTable;
        Vector<String> m_stringTable;
        Vector<String> m_hostAPITable;

        
    };

}
