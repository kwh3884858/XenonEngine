//  VirtualMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-4-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Engine/VirtualMachine/XenonStruction.h"
#include "Algorithms/String.h"
#include "Algorithms/Stack.h"

namespace Algorithm
{
    template<typename T>
    class StreamingVector;
}

namespace XenonEnigne
{
    using Algorithm::String;
    using Algorithm::Stack;
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
            enum ValuType
            {
                Interal,
                Float
            };
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
        const InstructionOp& GetInstructionByStackIndex(int index)const;
        const InstructionOp& ResolveInstructionOp(int instructionIndex, int opIndex)const;
        void PushFrame(int size);
        void PopFrame(int size);
        int ResolveOpAsInteger(int instructionIndex, int opIndex)const;
        float ResolveOpAsFloat(int instructionIndex, int opIndex)const;
        String ResolveOpAsString(int instructionIndex, int opIndex)const;
        //bool Laxer(const char* const content, Token** const tokens);
        ScriptHeader m_scriptHeader;
        Vector<Instruction*> m_instructionList;
        Vector<FunctionElement*> m_functionTable;
        Vector<LabelElement*> m_labelTable;
        Vector<String> m_stringTable;
        Vector<String> m_hostAPITable;

        Instruction m_returnValue;
        Stack<InstructionOp> m_localStack;
        int m_localCurrentFrameIndex;
        Stack<InstructionOp> m_glabalStack;
    };

}
