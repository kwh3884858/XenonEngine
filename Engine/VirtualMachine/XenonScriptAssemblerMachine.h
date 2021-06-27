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
            LexerState_Error,
            LexerState_Done,
            LexerState_Start,
            LexerState_Identifier,
            LexerState_String,
            LexerState_StringEnd,
            LexerState_Escape,
            LexerState_Integral,
            LexerState_Float,
            LexerState_Delimiter,
            LexerState_Comment
        };
        enum TokenType {
            None = 0,
            TokenType_IntergalIiteral,
            TokenType_FloatIiteral,
            TokenType_StringEntity,
            TokenType_Identifier,
            TokenType_Label,
            TokenType_Function,
            TokenType_HostAPI,
            TokenType_Register,
            TokenType_Keyword,
            TokenType_Delimiter,
            TokenTypeCount
        };

        static constexpr int keywordStringCount = 8;
        static const TypeString<int> keyWordString[keywordStringCount];

        enum KeyWord:short
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

        enum DelimiterWord
        {
            DelimiterWord_Colon = 0,          // A colon :
            DelimiterWord_SemiColon,          // A SemiColon ;
            DelimiterWord_OpenBracket,          // An openening bracket  [
            DelimiterWord_CloseBracket,          // An closing bracket    ]
            DelimiterWord_Comma,          // A comma,
            DelimiterWord_OpenBrace,          // An openening curly brace { 
            DelimiterWord_CloseBrace,          // An closing curly brace   }
            
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
            DelimiterSymbolState_Symbol,
            DelimiterSymbolState_DelimiterType
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
            InstructionOpType m_variableType = InstructionOpType::InstructionOpType_None;
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

        // Operand Type Bitfield Flags 
        typedef long long OpBitfiledFlag;
        const OpBitfiledFlag OP_FLAG_TYPE_NONE = 0;

        struct InstructionLookup
        {
            String m_mnemonic;
            KeyWord m_opType;
            int m_opCount = 0;
            Vector< OpBitfiledFlag > m_opFlags;
        };

        enum InstructionState
        {
            InstructionState_Start,
            InstructionState_Mnomonic,
            InstructionState_OpType,
            InstructionState_OpCount,
            InstructionState_OpTypeList
        };

        // Assembled Instruction Stream

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