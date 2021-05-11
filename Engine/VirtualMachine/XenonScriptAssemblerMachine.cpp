#include "XenonScriptAssemblerMachine.h"
#include <stdio.h>  // printf

namespace XenonEnigne
{
    XenonScriptAssemblerMachine::XenonScriptAssemblerMachine()
    {
    }

    XenonScriptAssemblerMachine::~XenonScriptAssemblerMachine()
    {
    }

    bool XenonScriptAssemblerMachine::InitializeInstructionList(const XenonFile * const xenonFile) 
    {
        unsigned int index = 0;
        bool isDone = false;
        bool isShouldAdd = true;
        InstructionState currentState = InstructionState::InstructionStateMnomonic;
        String tmpString;

        int tokenOpAmount = 0;
        int currentTokenOpCount = 0;

        Instruction* instrction;

        while (index < xenonFile->m_content.Count())
        {
             isDone = false;
             isShouldAdd = true;
             char currentChar = xenonFile->m_content[index];
            switch (currentState)
            {
            case InstructionStateStart:
            {
                if (IsCharIdent(currentChar))
                {
                    currentState = InstructionState::InstructionStateMnomonic;
                }
                else
                {
                    isShouldAdd = false;
                }
            }
                break;
            case InstructionStateMnomonic:
            {
                if (IsCharIdent(currentChar))
                {
                }
                else if (currentChar == SymbolComma) // ,
                {
                    isShouldAdd = false;
                    isDone = true;
                }
                else
                {
                    InstructionError(currentState, currentChar, index);
                    return false;
                }
            }
                break;
            case InstructionStateOpType:
            {
                if (IsCharNumeric(currentChar))
                {
                }
                else if (currentChar == SymbolComma)
                {
                    isShouldAdd = false;
                    isDone = true;
                }
                else
                {
                    InstructionError(currentState, currentChar, index);
                    return false;
                }
            }
                break;
            case InstructionStateOpCount:
            {
                if (IsCharNumeric(currentChar))
                {
                }
                if (IsNewLine(currentChar))
                {
                    isShouldAdd = false;
                    isDone = true;
                }
                else
                {
                    InstructionError(currentState, currentChar, index);
                    return false;
                }
            }
                break;
            case InstructionStateOpTypeList:
                if (IsCharNumeric(currentChar))
                {
                }
                else if (currentChar == SymbolComma)
                {
                    isShouldAdd = false;
                    isDone = true;
                }
                else if (IsNewLine(currentChar))
                {
                    currentTokenOpCount++;
                    isShouldAdd = false;
                    isDone = true;
                }
                else
                {
                    InstructionError(currentState, currentChar, index);
                    return false;
                }
                break;
            default:
                break;

            }

            if (isShouldAdd)
            {
                tmpString.Add(currentChar);
            }
            if (isDone)
            {
                switch (currentState)
                {
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateStart:
                    break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateMnomonic:
                {
                    instrction = new Instruction;
                    instrction->m_mnemonic = tmpString;
                    currentState = InstructionState::InstructionStateOpType;
                }
                    break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateOpType:
                {
                    instrction->m_opType = static_cast<KeyWord>( tmpString.ToInt() );
                    currentState = InstructionState::InstructionStateOpCount;
                }
                    break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateOpCount:
                {
                    instrction->m_opCount = tmpString.ToInt();
                    tokenOpAmount = instrction->m_opCount;
                    if (tokenOpAmount > 0)
                    {
                        currentTokenOpCount = 0;
                        for (int i = 0; i < tokenOpAmount; i++)
                        {
                            instrction->op.Add(OP_FLAG_TYPE_NONE);
                        }
                        currentState = InstructionState::InstructionStateOpTypeList;
                    }
                    else
                    {
                        m_instructionList.Add(instrction);
                        currentState = InstructionState::InstructionStateMnomonic;
                    }
                }
                    break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateOpTypeList:
                {
                    int bitOffset = tmpString.ToInt();
                    instrction->op[currentTokenOpCount] &= (1 << bitOffset);
                    if (currentTokenOpCount >= tokenOpAmount)
                    {
                        currentState = InstructionState::InstructionStateStart;
                    }
                }
                    break;
                default:
                    break;
                }
                currentState = CreateInstructionList(currentState, tmpString, instrction, tokenOpAmount, currentTokenOpCount);
                isDone = false;
                tmpString.Clear();
            }

            index++;
        }

        return true;
    }

    void XenonScriptAssemblerMachine::InitializeDelimiterList(const XenonFile*const xenonFile)
    {
        unsigned int index = 0;
        bool isDone = false;
        bool isShouldAdd = true;
        DelimiterSymbolState currentState = DelimiterSymbolState::Symbol;
        String tmpString;

        DelimiterSymbol* delimiterSymbol;

        while (index < xenonFile->m_content.Count())
        {
            char currentChar = xenonFile->m_content[index];
            UpdateCharacter(currentChar, isShouldAdd, isDone);

            if (isShouldAdd)
            {
                tmpString.Add(currentChar);
            }
            if (isDone)
            {
                currentState = CreateDelimiterList(currentState, tmpString, delimiterSymbol);
                isDone = false;
                tmpString.Clear();
            }

            index++;
            isShouldAdd = true;
        }
    }

    void XenonScriptAssemblerMachine::InstructionError(InstructionState state, char character, unsigned int index) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d", character, state, index);

    }

    void XenonScriptAssemblerMachine::UpdateCharacter(char currentChar, bool& isShouldAdd, bool& isDone) const
    {
        if (IsNewLine(currentChar) || currentChar == SymbolComma)
        {
            isShouldAdd = false;
            isDone = true;
        }
        if (IsCharWhitespace(currentChar))
        {
            isShouldAdd = false;
            isDone = false;
        }
        else
        {
            isShouldAdd = true;
        }
    }

    XenonEnigne::XenonScriptAssemblerMachine::InstructionState XenonScriptAssemblerMachine::CreateInstructionList(InstructionState currentState, const String& tmpString, Instruction*const instruction, int& tokenOpAmount, int& currentTokenopCount)
    {
        switch (currentState)
        {
        case InstructionStateMnomonic:
        {

        }
        break;
        case InstructionStateOpType:
        {

        }
        break;
        case InstructionStateOpCount:
        {

        }
        break;
        case InstructionStateOpTypeList:
        {
            if (currentTokenOpCount < tokenOpAmount)
            {

                currentTokenOpCount++;
            }
            else
            {
                m_instructionList->Add(instrction);
                currentState = InstructionState::InstructionStateMnomonic;
            }
        }
        break;
        default:
            break;
        }

        return currentState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::DelimiterSymbolState XenonScriptAssemblerMachine::CreateDelimiterList(DelimiterSymbolState currentState, const String& tmpString, DelimiterSymbol*& delimitSymbol)
    {
        switch (currentState)
        {
        case Symbol:
        {
            delimitSymbol = new DelimiterSymbol;
            delimitSymbol->m_symbol = tmpString.ToChar();
            currentState = DelimiterSymbolState::DelimiterType;
        }
        break;
        case DelimiterType:
        {
            delimitSymbol->m_tokenType = static_cast<DelimiterWord>( tmpString.ToInt() );
            currentState = DelimiterSymbolState::Symbol;
        }
        break;
        default:
            break;
        }
        return currentState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::GetNextToken(XenonFile*const xenonFile, unsigned int& refCurrentIndex, Token*const token) const
    {
        char currentCharacter = '\0';
        bool isShouldAddCharacter = true;
        LexerState currentLexerState = LexerState::LexerStateStart;
        bool isTokenDone = false;

        while (!isTokenDone) {
            isShouldAddCharacter = true;

            //Get Next Character
            currentCharacter = xenonFile->m_content[refCurrentIndex];
            if (currentCharacter >= xenonFile->m_content.Count())
            {
                return LexerState::LexerStateDone;
            }

            currentLexerState = DetermineLexerState(currentLexerState, currentCharacter, refCurrentIndex, isShouldAddCharacter, isTokenDone);

            refCurrentIndex++;

            if (isShouldAddCharacter)
            {
                token->m_character.Add(currentCharacter);
            }

            if (isTokenDone)
            {
                refCurrentIndex--;
                DetermineTokenType(token, currentLexerState);
                return currentLexerState;
            }
        }

        return currentLexerState;
    }

    XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::DetermineLexerState(LexerState lexerState, char character, unsigned int index, bool& isShouldAddCharacter, bool& isTokenDone) const
    {
        switch (lexerState)
        {
        case LexerState::LexerStateStart:
        {
            if (IsCharWhitespace(character))
            {
                isShouldAddCharacter = false;
            }
            if (SymbolSemicolon == SymbolSemicolon)
            {
                isShouldAddCharacter = false;
                lexerState = LexerState::LexerStateComment;
            }
            else if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerStateIntegral;
            }
            else if (IsCharIdent(character))
            {
                lexerState = LexerState::LexerStateIdentifier;
            }
            else if (IsCharDelimiter(character))
            {
                lexerState = LexerState::LexerStateDelimiter;
            }
            else if (character == SymbolQuote) // "
            {
                isShouldAddCharacter = false;
                lexerState = LexerState::LexerStateString;
            }
            else
            {
                return TokenError(lexerState, character, index);
            }
        }
        break;
        case LexerState::LexerStateIntegral:
        {
            if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerStateIntegral;
            }
            else if (IsCharFullStop(character))
            {
                lexerState = LexerState::LexerStateFloat;
            }
            else if (IsCharWhitespace(character))
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
            }
            else
            {
                return TokenError(lexerState, character, index);
            }
        }
        break;
        case LexerState::LexerStateFloat:
        {
            if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerStateFloat;
            }
            else if (IsCharWhitespace(character))
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
            }
            else
            {
                return TokenError(lexerState, character, index);
            }
        }
        break;
        case LexerStateError:
            break;
        case LexerStateDone:
            break;
        case LexerStateIdentifier:
        {
            if (IsCharIdent(character))
            {
                lexerState = LexerState::LexerStateIdentifier;
            }
            else if (IsCharWhitespace(character))
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
            }
            else
            {
                return TokenError(lexerState, character, index);
            }
        }
        break;
        case LexerStateString:
        {
            if (character == '"')
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
            }
            else if (character == SymbolBackslash)
            {
                isShouldAddCharacter = false;
                lexerState = LexerState::LexerStateEscape;
            }
        }
        break;
        case LexerStateEscape:
        {
            lexerState = LexerState::LexerStateString;
        }
        break;
        case LexerStateDelimiter:
        {
            isShouldAddCharacter = false;
            isTokenDone = true;
        }
        break;
        case LexerStateComment:
        {
            isShouldAddCharacter = false;
            if (IsNewLine(character))
            {
                lexerState = LexerState::LexerStateStart;
            }
        }
        break;
        default:
        {
            printf("Undefined State\n");
            TokenError(lexerState, character, index);
            return LexerState::LexerStateError;
        }


        }

        return lexerState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::TokenError(LexerState state, char character, unsigned int index) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d", character, state, index);
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
            for (int i = 0; i < m_instructionList.Count(); i++)
            {
                if (token->m_character == m_instructionList[i]->m_mnemonic)
                {
                    token->m_keyword = m_instructionList[i]->m_opType;
                    break;
                }
            }
            break;
        case LexerStateString:
            token->m_tokenType = TokenType::StringEntity;
            break;
        case LexerStateEscape:
            break;
        case LexerStateIntegral:
            token->m_tokenType = TokenType::Intergal;
            break;
        case LexerStateFloat:
            token->m_tokenType = TokenType::Float;
            break;
        case LexerStateDelimiter:
        {
            token->m_tokenType = TokenType::Identifier;
            for (int i = 0; i < m_delimiterList.Count(); i++)
            {
                if (token->m_character.ToChar() == m_delimiterList[i]->m_symbol)
                {
                    token->m_delimiter = m_delimiterList[i]->m_tokenType;
                    break;
                }
            }
        }
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

    void XenonScriptAssemblerMachine::Parsing(TokenVector*const tokenVector) const
{
        unsigned int stackSize = 0;
        unsigned int lineSize = 0;
        for (int i = 0; i < tokenVector->Count(); i++)
        {
            Token * token = (*tokenVector)[i];
            switch (token->m_tokenType)
            {
            case Intergal:
                break;
            case Float:
                break;
            case Identifier:
            {

            }
                break;
            case StringEntity:
                break;
            case Keyword:
            {
                switch (token->m_keyword)
                {
                    case  
                default:
                    break;
                }
            }
                break;
            case TokenTypeCount:
                break;
            }
        }


        delete tokenVector;
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

    bool XenonScriptAssemblerMachine::IsCharDelimiter(char character) const
    {
        if (
            character == SymbolColon ||
            character == SymbolOpenBracket ||
            character == SymbolCloseBracket ||
            character == SymbolOpenBrace ||
            character == SymbolCloseBrace ||
            character == SymbolSemicolon ||
            character == SymbolComma)
        {
            return true;
        }
        return false;
    }
}