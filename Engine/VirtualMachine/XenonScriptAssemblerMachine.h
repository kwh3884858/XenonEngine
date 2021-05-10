//  XenonScriptAssemblerMachine.h
//  XenonEngine
//
//  Created by whkong on 2021-5-8.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
#include "CrossPlatform/XenonFile.h"
namespace XenonEnigne
{
    using Algorithm::Vector;
    using CrossPlatform::XenonFile;
    using Algorithm::String;

    class XenonScriptAssemblerMachine
    {
        typedef Vector<Token*> TokenVector;
    public:
        XenonScriptAssemblerMachine();
        ~XenonScriptAssemblerMachine();
        void InitializeInstructionList(const XenonFile* xenonFile)const;

    private:
        void DetermineCharacterType(char c)const;
        void StripComment(XenonFile*const xenonFile, char commentChar)const;
        LexerState GetNextToken(XenonFile*const xenonFile, unsigned int& currentIndex, Token*const token)const;
        LexerState TokenError(char currentCharacter,LexerState currentState,unsigned int currentIndex)const;
        void DetermineTokenType(Token* const token, LexerState currentState)const;
        TokenVector* Lexer(XenonFile*const xenonFile)const;
        TokenVector* Parsing(XenonFile*const xenonFile)const;

        bool IsNewLine(char character)const;
        bool IsCharWhitespace(char character)const;
        bool IsCharNumeric(char character)const;
        bool IsCharIdent(char character)const;
        bool IsCharFullStop(char character)const;

        enum TokeType{
            TokenTypeInt            =   0 ,          // An integer literal
            TokenTypeFloat          =   1 ,          // An floating-point literal
            TokenTypeString         =   2 ,          // An string literal
            TokenTypeQuote          =   3 ,          // A double-quote
            TokenTypeIdent          =   4 ,          // An identifier
            TokenTypeColon          =   5 ,          // A colon :
            TokenTypeOpenBracket    =   6 ,          // An openening bracket  (
            TokenTypeCloseBracket   =   7 ,          // An closing bracket    )
            TokenTypeComma          =   8 ,          // A comma ,
            TokenTypeOpenBrace     =   9 ,          // An openening curly brace { 
            TokenTypeCloseBrace    =   10,          // An closing curly brace   }
            TokenTypeNew_Line       =   11,          // A newline
            TokenTypeInstr			=   12,			// An instruction
            TokenTypeSetStackSize   =   13,          // The SetStackSize directive
            TokenTypeVar            =   14,          // The Var/Var [] directives
            TokenTypeFunc           =   15,          // The Function directives
            TokenTypeParam          =   16,          // The Param directives
            TokenTypeReg_Retval     =   17,          // The _RetVal directives
            TokenTypeInvalid        =   18,          // Error code for invalid tokens
            END_OF_TOKEN_STREAM       =   19          // The end of the stream has been
            // reached
        }

        const char SymbolWhiteSpace = ' ';
        const char SymbolColon = ':';
        const char SymbolOpenBracket = '[';
        const char SymbolCloseBracket = ']';
        const char SymbolOpenBrace = '{';
        const char SymbolCloseBrace = '}';
        const char SymbolSemicolon = ';';
        const char SymbolQuote = '"';
        const char SymbolComma = ',';
        const char SymbolBackslash = '\\';
        const char SymbolSlash = '/';
        const char SymbolFullStop = '.';

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

        };
        enum TokenType {
            Intergal,
            Float,
            Identifier,
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

            AND    ,
            OR     ,
            XOR    ,
            NOT    ,
            SHL    ,
            SHR    ,

            CONCAT ,
            GETCHAR,
            SETCHAR,
            
            JMP    ,
            JE     ,
            JNE    ,
            JG     ,
            JL     ,
            JGE    ,
            JLE    ,
            
            PUSH   ,
            POP    ,
            
            CALL   ,
            RET    ,
            CALLHOS,
            
            PAUSE  ,
            EXIT   ,

            TokenTypeCount
        };
        

        struct StateChain {
            char m_symbol;
            LexerState m_fromState;
            LexerState m_toState;
        };

        struct Token
        {
            TokenType m_tokenType;
           Vector< char> m_character;
            
        };

        const int MaxInstructionMnemonicSize = 16;      // Maximum size of an instruction mnemonic's string
        



        // ---- Operand Type Bitfield Flags ---------------------------------------------------

    // The following constants are used as flags into an operand type bit field, hence
    // their values being increasing powers of 2.
        typedef int OpBitfiledFlag;
        const OpBitfiledFlag OP_FLAG_TYPE_NONE       = 0     ;
        const OpBitfiledFlag OP_FLAG_TYPE_INT        =1      ;      // Integer literal value
        const OpBitfiledFlag OP_FLAG_TYPE_FLOAT      =2      ;      // Floating-point literal value
        const OpBitfiledFlag OP_FLAG_TYPE_STRING     =4      ;      // Integer literal value
        const OpBitfiledFlag OP_FLAG_TYPE_MEM_REF    =8      ;      // Memory reference (variable or array
                                                                    // index; both absolute and relative)
        const OpBitfiledFlag OP_FLAG_TYPE_LINE_LABEL =16     ;      // Line label (used for jumps)
        const OpBitfiledFlag OP_FLAG_TYPE_FUNC_NAME  =32     ;      // Function table index (used for Call)
        const OpBitfiledFlag OP_FLAG_TYPE_HOST_API_CALL =  64 ;     // Host API Call table index (used for
                                                                    // CallHost)
 
        struct Instruction
        {
            String m_mnemonic;
            TokenType m_opType ;
            int m_opCount = 0;
            OpBitfiledFlag op = OP_FLAG_TYPE_NONE;
        };

        enum InstructionState
        {
            Mnomonic,
            OpType,
            OpCount,
            OpTypeList
        };

        Vector<Instruction*> m_instructionList;
        Vector<StateChain> m_stateTransformList;

        // Ready for deletion
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