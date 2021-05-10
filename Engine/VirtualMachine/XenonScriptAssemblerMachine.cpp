#include "XenonScriptAssemblerMachine.h"

namespace XenonEnigne
{
    using EndOfLine = CrossPlatform::CL;
    XenonScriptAssemblerMachine::XenonScriptAssemblerMachine()
    {
    }

    XenonScriptAssemblerMachine::~XenonScriptAssemblerMachine()
    {
    }

    void XenonScriptAssemblerMachine::InitializeInstructionList(const XenonFile* xenonFile) const
    {
        unsigned int index = 0;
        bool isDone = false;
        bool isShouldAdd = true;
        InstructionState currentState = InstructionState::Mnomonic;
        String tmpString;

        int tokenOpAmount = 0;
        int currentTokenOpCount = 0;

        Instruction* instrction;

        while (index < xenonFile->m_content.Count())
        {
            char currentChar = xenonFile->m_content[index];
            if (IsNewLine(currentChar))
            {
                isShouldAdd = false;
                isDone = true;
            }
            if (IsCharWhitespace(currentChar))
            {
                isShouldAdd = false;
                isDone = false;
            }
            else if (currentChar == ',')
            {
                isShouldAdd = false;
                isDone = true;
            }
            else
            {
                isShouldAdd = true;
            }

            if (isShouldAdd)
            {
                tmpString.Add(currentChar);
            }
            if (isDone)
            {             
                switch (currentState)
                {

                case Mnomonic:
                {
                    instrction = new Instruction;
                    instrction->m_mnemonic = tmpString;
                    currentState = InstructionState::OpType;
                }
                break;
                case OpType:
                {
                    instrction->m_opType = tmpString.ToInt();
                    currentState = InstructionState::OpCount;
                }
                break;
                case OpCount:
                {
                    instrction->m_opCount = tmpString.ToInt();
                    tokenOpAmount = instrction->m_opCount;
                    if (tokenOpAmount > 0)
                    {
                        currentTokenOpCount = 0;
                        currentState = InstructionState::OpTypeList;
                    }
                    else
                    {
                        instrction->op = OP_FLAG_TYPE_NONE;
                        m_instructionList->Add(instrction);
                        currentState = InstructionState::Mnomonic;
                    }
                }
                    break;
                case OpTypeList:
                {
                    if (currentTokenOpCount < tokenOpAmount)
                    {
                        int bitOffset = tmpString.ToInt();
                        instrction->op &= (1 << bitOffset);
                        currentTokenOpCount++;
                    }
                    else
                    {
                        m_instructionList->Add(instrction);
                        currentState = InstructionState::Mnomonic;
                    }
                }

                break;
                default:
                    break;


                }
                isDone = false;
                tmpString.Clear();
            }

            index++;
            isShouldAdd = true;
        }
    }

    void XenonScriptAssemblerMachine::StripComment(XenonFile*const raw, char commentChar) const
    {
        LexerState state = LexerState::LexerStateIdle;
        char content = raw->m_content;
        for (int i = 0; i < raw->m_lineIndex.Count(); i++)
        {
            int index = raw->m_lineIndex[i];
            while (content[index] != EndOfLine)
            {
                switch (content[index])
                {
                case SymbolQuote:
                {
                    if (state == LexerState::LexerStateString)
                    {
                        state = LexerState::LexerStateIdle;
                    }
                    else
                    {
                        state = LexerState::LexerStateString;
                    }
                }
                break;
                case commentChar:
                {
                    if (state != LexerState::LexerStateString)
                    {
                        content[index] = EndOfLine;
                    }
                }
                break;
                case SymbolBackslash:
                {
                    state =
                }
                break;
                default:
                    break;
                }
                index++;
            }
        }
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::GetNextToken(XenonFile*const xenonFile, unsigned int& currentIndex, Token*const token) const
    {
        char currentCharacter = '\0';
        bool isShouldAddCharacter = true;
        LexerState currentLexerState = LexerState::LexerStateStart;
        bool isTokenDone = false;

        while (!isTokenDone) {
            isShouldAddCharacter = true;

            //Get Next Character
            currentCharacter = xenonFile->m_content[currentIndex];
            if (currentCharacter >= xenonFile->m_content.Count())
            {
                return LexerState::LexerStateDone;
            }

            switch (currentLexerState)
            {
            case LexerState::LexerStateStart:
            {
                if (IsCharWhitespace(currentCharacter))
                {
                    isShouldAddCharacter = false;
                }
                else if (IsCharNumeric(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateIntegral;
                }
                else if (IsCharIdent(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateIdentifier;
                }
                else
                {
                    return TokenError(currentCharacter, currentLexerState, currentIndex);
                }
            }
            break;
            case LexerState::LexerStateIntegral:
            {
                if (IsCharNumeric(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateIntegral;
                }
                else if (IsCharFullStop(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateFloat;
                }
                else if (IsCharWhitespace(currentCharacter))
                {
                    isShouldAddCharacter = false;
                    isTokenDone = true;
                }
                else
                {
                    return TokenError(currentCharacter, currentLexerState, currentIndex);
                }
            }
            break;
            case LexerState::LexerStateFloat:
            {
                if (IsCharNumeric(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateFloat;
                }
                else if (IsCharWhitespace(currentCharacter))
                {
                    isShouldAddCharacter = false;
                    isTokenDone = true;
                }
                else
                {
                    return TokenError(currentCharacter, currentLexerState, currentIndex);
                }
            }
            break;
            case LexerStateError:
                break;
            case LexerStateDone:
                break;
            case LexerStateIdentifier:
            {
                if (IsCharIdent(currentCharacter))
                {
                    currentLexerState = LexerState::LexerStateIdentifier;
                }
                else if (IsCharWhitespace(currentCharacter))
                {
                    isShouldAddCharacter = false;
                    isTokenDone = true;
                }
                else
                {
                    return TokenError(currentCharacter, currentLexerState, currentIndex);
                }
            }
            break;
            case LexerStateString:
                break;
            case LexerStateEscape:
                break;
            case LexerStateDelimiter:
            {

            }
                break;
            default:
                printf("Undefined State\n");
                return nullptr;
                break;


            }

            currentIndex++;

            if (isShouldAddCharacter)
            {
                token->m_character.Add(currentCharacter);
            }

            if (isTokenDone)
            {
                DetermineTokenType(token, currentLexerState);
                return currentLexerState;
            }
        }

        return currentLexerState;
    }

    LexerState XenonScriptAssemblerMachine::TokenError(char currentCharacter, LexerState currentState, unsigned int currentIndex) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d", currentCharacter, currentLexerState, currentIndex);
        return LexerState::LexerStateError;
    }

    void XenonScriptAssemblerMachine::DetermineTokenType(Token* const token, LexerState currentState) const
    {
        switch (currentState)
        {
        default:
            break;
        case LexerStateError:
            break;
        case LexerStateDone:
            break;
        case LexerStateStart:
            break;
        case LexerStateIdentifier:
            token->m_tokenType = TokenType::Identifier;
            for (int i = 0 ; i < m_instructionList.Count(); i ++)
            {
                if (token->m_character == m_instructionList[i]->m_mnemonic)
                {
                    token->m_tokenType = m_instructionList[i]->m_opType;
                    break;
                }
            }
            break;
        case LexerStateString:
            break;
        case LexerStateEscape:
            break;
        case LexerStateIntegral:
            token->m_tokenType = TokenType::Intergal;
            break;
        case LexerStateFloat:
            token->m_tokenType = TokenType::Float;
            break;
        }
    }

    XenonScriptAssemblerMachine::TokenVector* XenonScriptAssemblerMachine::Lexer(XenonFile*const xenonFile) const
    {
        TokenVector* tokenVector = new TokenVector;
        unsigned int currentIndex = 0;
        while (currentIndex < xenonFile->m_content.Count()) {
            Token* token = new Token;
            LexerState lexerState = GetNextToken(xenonFile, currentIndex, token);
            switch (lexerState)
            {
            case LexerState::LexerStateDone:
                return tokenVector;
            case LexerState::LexerStateError:
                return nullptr;
            default:
                tokenVector->Add(token);
                break;
            }
        }
    }

    XenonScriptAssemblerMachine::TokenVector* XenonScriptAssemblerMachine::Parsing(XenonFile*const xenonFile) const
    {

    }

    bool XenonScriptAssemblerMachine::IsNewLine(char character) const
    {
        if (character == '\t' || character == '\r')
            return true;
        else
            return false;
    }

    bool XenonScriptAssemblerMachine::IsCharWhitespace(char character) const
    {
        // Return true if the character is a space or tab.

        if (character == ' ' || character == '\t' || character == '\r')
            return true;
        else
            return false;
    }

    bool XenonScriptAssemblerMachine::IsCharNumeric(char character) const
    {
        // Return true if the character is between 0 and 9 inclusive.

        if (character >= '0' && character <= '9')
            return true;
        else
            return false;
    }

    bool XenonScriptAssemblerMachine::IsCharIdent(char character) const
    {
        // Return true if the character is between 0 or 9 inclusive or is an uppercase or
        // lowercase letter or underscore

        if ((character >= '0' && character <= '9') ||
            (character >= 'A' && character <= 'Z') ||
            (character >= 'a' && character <= 'z') ||
            character == '_')
            return true;
        else
            return false;
    }

    bool XenonScriptAssemblerMachine::IsCharFullStop(char character) const
    {
        return character == SymbolFullStop;
    }

}