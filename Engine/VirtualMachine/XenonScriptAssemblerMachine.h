//  XenonScriptAssemblerMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-5-8.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
#include "CrossPlatform/XenonFile.h"
#include "Algorithms/TypeString.h"
#include "Engine/VirtualMachine/XenonStruction.h"

namespace XenonEnigne
{
    using Algorithm::Vector;
    using CrossPlatform::XenonFile;
    using Algorithm::String;
    using Algorithm::TypeString;

    class XenonScriptAssemblerMachine
    {
    public:
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

        static constexpr int keywordStringCount = 8;
        static const TypeString<int> keyWordString[keywordStringCount];

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

        static constexpr char Main_Function_Name[] ="Main";

        struct SymbolElement
        {
            InstructionOpType m_variableType = InstructionOpType::InstructionOpType_None;
            Token* m_symbolToken = nullptr;
            unsigned int m_size = 0;
            int m_stackIndex = 0;
            unsigned int m_functionIndex = 0;
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