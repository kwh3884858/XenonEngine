//  XenonScriptAssemblerMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-5-8.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
#include "CrossPlatform/XenonFile.h"
#include "Algorithms/TypeString.h"
namespace XenonEnigne
{
    using Algorithm::Vector;
    using CrossPlatform::XenonFile;
    using Algorithm::String;
    using Algorithm::TypeString;

    class XenonScriptAssemblerMachine
    {

    public:
        XenonScriptAssemblerMachine();
        ~XenonScriptAssemblerMachine();

        bool InitializeInstructionList(const XenonFile * const xenonFile);
        void InitializeDelimiterList(const XenonFile*const xenonFile);
    private:

        const char SymbolWhiteSpace = ' ';
        const char SymbolColon = ':';
        const char SymbolOpenBracket = '[';
        const char SymbolCloseBracket = ']';
        const char SymbolOpenBrace = '{';
        const char SymbolCloseBrace = '}';
        const char SymbolSemicolon = ';';
        const char SymbolComma = ',';
        const char SymbolQuote = '"';
        const char SymbolBackslash = '\\';
        const char SymbolFullStop = '.';
        //const char SymbolSlash = '/';

        enum LexerState {
            LexerStateError,
            LexerStateDone,
            LexerStateStart,
            LexerStateIdentifier,
            LexerStateString,
            LexerStateEscape,
            LexerStateIntegral,
            LexerStateFloat,
            LexerStateDelimiter,
            LexerStateComment,
        };
        enum TokenType {
            None = -1,
            IntergalIiteral,
            FloatIiteral,
            StringEntity,
            Identifier,
            Label,
            Function,
            HostAPI,
            Register,
            Keyword,
            Delimiter,
            TokenTypeCount
        };

        static const TypeString<int> keyWordString[] =
        {
            {"int",  TokenType::IntergalIiteral},
            {"float", TokenType::FloatIiteral},
            {"string",  TokenType::StringEntity},
            {"identifier",  TokenType::Identifier},
            {"label",  TokenType::Label},
            {"function",  TokenType::Function},
            {"hostAPI",  TokenType::HostAPI},
            {"register",   TokenType::Register},
        };
        enum KeyWord
        {
            INT,
            FLOAT,

            MOV = 0,
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            EXP,
            NEG,
            INC,
            DEC,

            AND,
            OR,
            XOR,
            NOT,
            SHL,
            SHR,

            CONCAT,
            GETCHAR,
            SETCHAR,

            JMP,
            JE,
            JNE,
            JG,
            JL,
            JGE,
            JLE,

            PUSH,
            POP,

            FUNC,
            PARAM,
            CALL,
            RET,
            CALLHOS,

            PAUSE,
            EXIT
        };

        enum DelimiterWord
        {
            Colon = 0,          // A colon :
            SemiColon,          // A SemiColon ;
            OpenBracket,          // An openening bracket  [
            CloseBracket,          // An closing bracket    ]
            Comma,          // A comma,
            OpenBrace,          // An openening curly brace { 
            CloseBrace,          // An closing curly brace   }

        };

        enum InstructionOpType
        {
            InstructionOpTypeNone = 0,
            InstructionOpTypeInteralLiteral,           // Integer literal value
            InstructionOpTypeFloatLiteral,           // Floating-point literal value
            InstructionOpTypeStringIndex,           // String literal value
            InstructionOpTypeAbsoluteStackIndex,           // Absolute array index
            InstructionOpTypeRelativeStackIndex,           // Relative array index
            InstructionOpTypeInstructionIndex,           // Instruction index
            InstructionOpTypeFunctionIndex,           // Function index
            InstructionOpTypeHostAPICallIndex,           // Host API call index
            InstructionOpTypeRegister,           // Register
        };

        struct Token
        {
            TokenType m_tokenType;
            union
            {
                KeyWord m_keyword;
                DelimiterWord m_delimiter;
            };
            String m_character;
        };

        struct DelimiterSymbol
        {
            char m_symbol;
            DelimiterWord m_tokenType;
        };
        enum DelimiterSymbolState
        {
            Symbol,
            DelimiterType
        };

        struct ScriptHeader
        {
            unsigned int m_globalDataSize = 0;
            int m_mainFunctionEntryIndex = -1;
        };

        const String Main_Function_Name = "_Main";

        struct FunctionElement
        {
            Token* m_functionToken = nullptr;
            int m_functionIndex = -1;
            int m_entryPoint = -1;
            int m_localStackSize = 0;
            int m_parameterCount = 0;

        };

        struct SymbolElement
        {
            InstructionOpType m_variableType = InstructionOpType::InstructionOpTypeNone;
            Token* m_symbolToken = nullptr;
            unsigned int m_size = 0;
            int m_stackIndex = 0;
            unsigned int m_functionIndex = 0;
        };

        struct LabelElement
        {
            Token* m_token = nullptr;
            unsigned int m_instructionStreamIndex = 0;
            FunctionElement* m_currentFunction = 0;
        };

        // ---- Operand Type Bitfield Flags ---------------------------------------------------

    // The following constants are used as flags into an operand type bit field, hence
    // their values being increasing powers of 2.
        typedef long long OpBitfiledFlag;
        const OpBitfiledFlag OP_FLAG_TYPE_NONE = 0;
        const OpBitfiledFlag OP_FLAG_TYPE_INT = 1;      // Integer literal value
        const OpBitfiledFlag OP_FLAG_TYPE_FLOAT = 2;      // Floating-point literal value
        const OpBitfiledFlag OP_FLAG_TYPE_STRING = 4;      // Integer literal value
        const OpBitfiledFlag OP_FLAG_TYPE_MEM_REF = 8;      // Memory reference (variable or array
                                                                    // index; both absolute and relative)
        const OpBitfiledFlag OP_FLAG_TYPE_LINE_LABEL = 16;      // Line label (used for jumps)
        const OpBitfiledFlag OP_FLAG_TYPE_FUNC_NAME = 32;      // Function table index (used for Call)
        const OpBitfiledFlag OP_FLAG_TYPE_HOST_API_CALL = 64;     // Host API Call table index (used for
                                                                    // CallHost)
        struct InstructionLookup
        {
            String m_mnemonic;
            KeyWord m_opType;
            int m_opCount = 0;
            Vector< OpBitfiledFlag > m_opFlags;
        };

        enum InstructionState
        {
            InstructionStateStart,
            InstructionStateMnomonic,
            InstructionStateOpType,
            InstructionStateOpCount,
            InstructionStateOpTypeList
        };

        // Assembled Instruction Stream

        struct InstructionOp
        {
            InstructionOpType m_type = InstructionOpType::InstructionOpTypeNone;                                  // Type
            union                                       // The value
            {
                int m_interalLiteral;                        // Integer literal
                float m_floatLiteral;                    // Float literal
                int m_stringTableIndex;                  // String table index
                int m_stackIndex;                        // Stack index
                int m_instructionIndex;                        // Instruction index
                int m_funcIndex;                         // Function index
                int m_hostAPICallIndex;                  // Host API Call index
                int m_reg;                               // Register code
            };
            int m_offsetIndex;                           // Index of the offset
        };

        struct Instruction
        {
            KeyWord m_opCode;
            unsigned int m_opCount;
            Vector<InstructionOp*> m_ops;
        };

        typedef Vector<Token*> TokenVector;

        void InstructionError(InstructionState state, char character, unsigned int index)const;
        void UpdateInstuctionCharacter(char currentCharacter, bool& isShouldAdd, bool& isDone)const;
        void UpdateCharacter(char currentChar, bool& isShouldAdd, bool& isDone)const;
        DelimiterSymbolState CreateDelimiterList(DelimiterSymbolState currentState, const String& tmpString, DelimiterSymbol*& delimitSymbol);
        void DetermineCharacterType(char c)const;

        LexerState GetNextToken(XenonFile* const xenonFile, unsigned int& refCurrentIndex, Token* const token)const;
        LexerState DetermineLexerState(LexerState lexerState, char character, unsigned int index, bool& isShouldAddCharacter, bool& isTokenDone)const;
        LexerState TokenError(LexerState state, char character, unsigned int index)const;
        void DetermineTokenType(Token* const token, LexerState currentState)const;

        TokenVector* Lexer(XenonFile* const xenonFile)const;
        bool Parsing(TokenVector* const tokenVector);
        bool BuildSymbolAndFunctionAndLabelTable(TokenVector* const tokenVector);
        void CreateInstructionList(TokenVector* const const tokenVector, const Vector<Instruction *>& instructionStream);

        bool CreateSymbol(TokenVector* const tokenVector, Token* currentToken, InstructionOpType tokenType, FunctionElement* const functionElement, unsigned int& refIndex, unsigned int& refGlobalStackSize);

        bool IsNewLine(char character)const;
        bool IsCharWhitespace(char character)const;
        bool IsCharNumeric(char character)const;
        bool IsCharIdent(char character)const;
        bool IsCharFullStop(char character)const;
        bool IsCharDelimiter(char character)const;

        Token* MoveToNextToken(const TokenVector& tokenVector, unsigned int& index)const;
        SymbolElement*const GetSymbolByName(const String& symbolName)const;
        FunctionElement*const GetFunctionByName(const String& functionName) const;
        LabelElement*const GetLabelByName(const String& labelName);
        InstructionLookup*const GetInstructionByKeyword(const KeyWord& keyword) const;

        unsigned int Local_Stack_Start_Index = 2;

        ScriptHeader m_scriptHeader;

        Vector<DelimiterSymbol*> m_delimiterList;
        Vector<InstructionLookup*> m_instructionLookupList;

        // For Paring
        Vector<SymbolElement*> m_symbolTable;
        Vector<FunctionElement*> m_functionTable;
        Vector<LabelElement*> m_labelTable;
        Vector<String> m_stringTable;
        Vector<Instruction*> m_instructionList;

        // Ready for deletion///////////////////////////////////
        const int MaxInstructionMnemonicSize = 16;      // Maximum size of an instruction mnemonic's string

        enum TokeType {
            TokenTypeInt = 0,          // An integer literal
            TokenTypeFloat = 1,          // An floating-point literal
            TokenTypeString = 2,          // An string literal
            TokenTypeQuote = 3,          // A double-quote
            TokenTypeIdent = 4,          // An identifier
            TokenTypeColon = 5,          // A colon :
            TokenTypeOpenBracket = 6,          // An openening bracket  (
            TokenTypeCloseBracket = 7,          // An closing bracket    )
            TokenTypeComma = 8,          // A comma ,
            TokenTypeOpenBrace = 9,          // An openening curly brace { 
            TokenTypeCloseBrace = 10,          // An closing curly brace   }
            TokenTypeNew_Line = 11,          // A newline
            TokenTypeInstr = 12,			// An instruction
            TokenTypeSetStackSize = 13,          // The SetStackSize directive
            TokenTypeVar = 14,          // The Var/Var [] directives
            TokenTypeFunc = 15,          // The Function directives
            TokenTypeParam = 16,          // The Param directives
            TokenTypeReg_Retval = 17,          // The _RetVal directives
            TokenTypeInvalid = 18,          // Error code for invalid tokens
            END_OF_TOKEN_STREAM = 19          // The end of the stream has been
            // reached
        }
        enum InstrctionOpCode {
            INSTR_MOV = 0,

            INSTR_ADD = 1,
            INSTR_SUB = 2,
            INSTR_MUL = 3,
            INSTR_DIV = 4,
            INSTR_MOD = 5,
            INSTR_EXP = 6,
            INSTR_NEG = 7,
            INSTR_INC = 8,
            INSTR_DEC = 9,

            INSTR_AND = 10,
            INSTR_OR = 11,
            INSTR_XOR = 12,
            INSTR_NOT = 13,
            INSTR_SHL = 14,
            INSTR_SHR = 15,

            INSTR_CONCAT = 16,
            INSTR_GETCHAR = 17,
            INSTR_SETCHAR = 18,

            INSTR_JMP = 19,
            INSTR_JE = 20,
            INSTR_JNE = 21,
            INSTR_JG = 22,
            INSTR_JL = 23,
            INSTR_JGE = 24,
            INSTR_JLE = 25,

            INSTR_PUSH = 26,
            INSTR_POP = 27,

            INSTR_CALL = 28,
            INSTR_RET = 29,
            INSTR_CALLHOST = 30,

            INSTR_PAUSE = 31,
            INSTR_EXIT = 32
        };

        const char InstrcutionMOV[] = "MOV";
        const char InstrcutionADD[] = "ADD";
        const char InstrcutionSUB[] = "SUB";
        const char InstrcutionMUL[] = "MUL";
        const char InstrcutionDIV[] = "DIV";
        const char InstrcutionMOD[] = "MOD";
        const char InstrcutionEXP[] = "EXP";
        const char InstrcutionNEG[] = "NEG";
        const char InstrcutionINC[] = "INC";
        const char InstrcutionDEC[] = "DEC";
        const char InstrcutionAND[] = "AND";
        const char InstrcutionOR[] = "OR ";
        const char InstrcutionXOR[] = "XOR";
        const char InstrcutionNOT[] = "NOT";
        const char InstrcutionSHL[] = "SHL";
        const char InstrcutionSHR[] = "SHR";
        const char InstrcutionCONCAT[] = "CONAT";
        const char InstrcutionGETCHAR[] = "GETHAR";
        const char InstrcutionSETCHAR[] = "SETHAR";
        const char InstrcutionJMP[] = "JMP";
        const char InstrcutionJE[] = "JE ";
        const char InstrcutionJNE[] = "JNE";
        const char InstrcutionJG[] = "JG ";
        const char InstrcutionJL[] = "JL ";
        const char InstrcutionJGE[] = "JGE";
        const char InstrcutionJLE[] = "JLE";
        const char InstrcutionPUSH[] = "PUS";
        const char InstrcutionPOP[] = "POP";
        const char InstrcutionCALL[] = "CAL";
        const char InstrcutionRET[] = "RET";
        const char InstrcutionCALLHOS[] = "CALLHOS";
        const char InstrcutionPAUSE[] = "PASUE";
        const char InstrcutionEXIT[] = "EXI";
    };

}