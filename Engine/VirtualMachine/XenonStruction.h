#pragma once
namespace XenonEnigne
{
    struct XenonScriptAssemblerMachine::Token;

    struct ScriptHeader
    {
        unsigned int m_globalDataSize = 0;
        int m_mainFunctionEntryIndex = -1;
    };
    
    enum KeyWord :short
    {
        KeyWord_INT,
        KeyWord_FLOAT,

        KeyWord_MOV,
        KeyWord_ADD,
        KeyWord_SUB,
        KeyWord_MUL,
        KeyWord_DIV,
        KeyWord_MOD,
        KeyWord_EXP,
        KeyWord_NEG,
        KeyWord_INC,
        KeyWord_DEC,

        KeyWord_AND,
        KeyWord_OR,
        KeyWord_XOR,
        KeyWord_NOT,
        KeyWord_SHL,
        KeyWord_SHR,

        KeyWord_CONCAT,
        KeyWord_GETCHAR,
        KeyWord_SETCHAR,

        KeyWord_JMP,
        KeyWord_JE,
        KeyWord_JNE,
        KeyWord_JG,
        KeyWord_JL,
        KeyWord_JGE,
        KeyWord_JLE,

        KeyWord_PUSH,
        KeyWord_POP,

        KeyWord_FUNC,
        KeyWord_PARAM,
        KeyWord_CALL,
        KeyWord_RET,
        KeyWord_CALLHOST,

        KeyWord_PAUSE,
        KeyWord_EXIT
    };


    enum InstructionOpType
    {
        InstructionOpType_None = 0,
        InstructionOpType_InteralLiteral,           // Integer literal value
        InstructionOpType_FloatLiteral,           // Floating-point literal value
        InstructionOpType_StringIndex,           // String literal value
        InstructionOpType_AbsoluteStackIndex,           // Absolute array index
        InstructionOpType_RelativeStackIndex,           // Relative array index
        InstructionOpType_InstructionIndex,           // Instruction index
        InstructionOpType_FunctionIndex,           // Function index
        InstructionOpType_HostAPICallIndex,           // Host API call index
        InstructionOpType_Register,           // Register
    };

    struct InstructionOp
    {
        InstructionOpType m_type = InstructionOpType::InstructionOpType_None;                                  // Type
        union                                        // The value
        {
            int m_interalLiteral;                    // Integer literal
            float m_floatLiteral;                    // Float literal
            int m_stringTableIndex;                  // String table index
            int m_stackIndex;                        // Stack index
            int m_instructionIndex;                  // Instruction index
            int m_funcIndex;                         // Function index
            int m_hostAPICallIndex;                  // Host API Call index
            int m_reg;                               // Register code
        };
        int m_offsetIndex;                           // Index of the offset
    };

    // Assembled Instruction Stream
    struct Instruction
    {
        KeyWord m_opCode;
        unsigned int m_opCount;
        Vector<InstructionOp*> m_ops;
    };

    struct FunctionElement
    {
        XenonScriptAssemblerMachine::Token* m_functionToken = nullptr;
        int m_functionIndex = -1;
        int m_entryPoint = -1;
        int m_localStackSize = 0;
        int m_parameterCount = 0;
    };

    struct LabelElement
    {
        XenonScriptAssemblerMachine::Token* m_token = nullptr;
        unsigned int m_instructionStreamIndex = 0;
        FunctionElement* m_currentFunction = 0;
    };
}