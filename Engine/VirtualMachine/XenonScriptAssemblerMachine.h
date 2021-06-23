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
            LexerStateStringEnd,
            LexerStateEscape,
            LexerStateIntegral,
            LexerStateFloat,
            LexerStateDelimiter,
            LexerStateComment,
        };
        enum TokenType {
            None = 0,
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

        static constexpr int keywordStringCount = 8;
        static const TypeString<int> keyWordString[keywordStringCount];

        enum KeyWord:short
        {
            INT,
            FLOAT,

            MOV,
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
            CALLHOST,

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
            String m_character;
            TokenType m_tokenType;
            union
            {
                KeyWord m_keyword;
                DelimiterWord m_delimiter;
            };
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

        static constexpr char Main_Function_Name[] ="Main";

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


    public:
        XenonScriptAssemblerMachine();
        ~XenonScriptAssemblerMachine();

        bool InitializeInstructionList(const XenonFile* const xenonFile);
        bool InitializeDelimiterList(const XenonFile* const xenonFile);
        void Compiler(const XenonFile* const xenonFile);        
        void BuildXEX(XenonFile*const xenonFile)const;
    private:

        typedef Vector<Token*> TokenVector;

        void InstructionError(InstructionState state, char character, int index, int lineISize)const;
        void UpdateInstuctionCharacter(char currentCharacter, bool& isShouldAdd, bool& isDone)const;
        void UpdateCharacter(char currentChar, bool& isShouldAdd, bool& isDone)const;
        DelimiterSymbolState CreateDelimiterList(DelimiterSymbolState currentState, const String& tmpString, DelimiterSymbol*& delimitSymbol);
        void DetermineCharacterType(char c)const;

        LexerState GetNextToken(const XenonFile * const xenonFile, int& refCurrentIndex, Token* const token)const;
        LexerState DetermineLexerState(LexerState lexerState, char character, unsigned int index, bool& isShouldAddCharacter, bool& isTokenDone, int& refLineCountForDebug)const;
        LexerState TokenError(LexerState state, char character, unsigned int index, int lineCountForDebug)const;
        void DetermineTokenType(Token* const token, LexerState currentState)const;

        TokenVector* Lexer(const XenonFile * const xenonFile)const;
        bool Parsing(TokenVector* const tokenVector);
        bool BuildSymbolAndFunctionAndLabelTable(TokenVector* const tokenVector);
        void BuildTableError(const Token*const token, int index);
        bool CreateInstructionList(TokenVector* const tokenVector, const Vector<Instruction*>& instructionStream);
        void CreateInstructionListError(const Token* const token, int index);
        void PrintAssemblerState(XenonFile*const xenonFile)const;

        bool CreateSymbol(TokenVector* const tokenVector, Token* currentToken, InstructionOpType tokenType, FunctionElement* const functionElement, int& refIndex, unsigned int& refGlobalStackSize);

        bool IsNewLine(char character)const;
        bool IsCharWhitespace(char character)const;
        bool IsCharNumeric(char character)const;
        bool IsCharIdent(char character)const;
        bool IsCharFullStop(char character)const;
        bool IsCharDelimiter(char character)const;

        Token* MoveToNextToken(const TokenVector& tokenVector, int& index)const;
        Token* PeekNextToken(const TokenVector& tokenVector, int& index)const;
        SymbolElement* const GetSymbolByName(const String& symbolName)const;
        FunctionElement* const GetFunctionByName(const String& functionName) const;
        LabelElement* const GetLabelByName(const String& labelName);
        InstructionLookup* const GetInstructionByKeyword(const KeyWord& keyword) const;

        unsigned int Local_Stack_Start_Index = 2;

        ScriptHeader m_scriptHeader;

        Vector<DelimiterSymbol*> m_delimiterList;
        Vector<InstructionLookup*> m_instructionLookupList;

        // For Paring
        Vector<SymbolElement*> m_symbolTable;
        Vector<FunctionElement*> m_functionTable;
        Vector<LabelElement*> m_labelTable;
        Vector<String> m_stringTable;
        Vector<String> m_hostAPITable;
        Vector<Instruction*> m_instructionList;
    };
}