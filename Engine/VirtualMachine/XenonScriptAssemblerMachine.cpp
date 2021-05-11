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

    bool XenonScriptAssemblerMachine::InitializeInstructionList(const XenonFile* xenonFile) const
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
                if (IsNewLine(currentChar))
                {
                    isShouldAdd = false;
                    isDone = true;
                }
                if (IsCharWhitespace(currentChar))
                {
                    isShouldAdd = false;
                }
            }
                break;
            case InstructionStateOpCount:
                break;
            case InstructionStateOpTypeList:
                break;
            default:
                break;

            }
            char currentChar = xenonFile->m_content[index];
            UpdateCharacter(currentChar, isShouldAdd, isDone);

            if (isShouldAdd)
            {
                tmpString.Add(currentChar);
            }
            if (isDone)
            {
                currentState = CreateInstructionList(currentState, tmpString, instrction, tokenOpAmount, currentTokenOpCount);
                isDone = false;
                tmpString.Clear();
            }

            index++;
            isShouldAdd = true;
        }

        return true;
    }

    void XenonScriptAssemblerMachine::InitializeDelimiterList(const XenonFile*const xenonFile) const
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
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d", character, currentLexerState, index);

    }

    void XenonScriptAssemblerMachine::UpdateCharacter(char currentCharacter, bool& isShouldAdd, bool& isDone) const
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
            instrction = new Instruction;
            instrction->m_mnemonic = tmpString;
            currentState = InstructionState::InstructionStateOpType;
        }
        break;
        case InstructionStateOpType:
        {
            instrction->m_opType = tmpString.ToInt();
            currentState = InstructionState::InstructionStateOpCount;
        }
        break;
        case InstructionStateOpCount:
        {
            instrction->m_opCount = tmpString.ToInt();
            tokenOpAmount = instrction->m_opCount;
            if (tokenOpAmount > 0)
            {
                currentTokenOpCount = 0;
                currentState = InstructionState::InstructionStateOpTypeList;
            }
            else
            {
                instrction->op = OP_FLAG_TYPE_NONE;
                m_instructionList->Add(instrction);
                currentState = InstructionState::InstructionStateMnomonic;
            }
        }
        break;
        case InstructionStateOpTypeList:
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
                currentState = InstructionState::InstructionStateMnomonic;
            }
        }
        break;
        default:
            break;
        }

        return currentState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::DelimiterSymbolState XenonScriptAssemblerMachine::CreateDelimiterList(DelimiterSymbolState currentState, const String& tmpString, DelimiterSymbol*const delimitSymbol)
    {
        switch (currentState)
        {
        case Symbol:
        {
            delimitSymbol = new DelimiterSymbol;
            delimitSymbol->m_symbol = tmpString;
            currentState = InstructionState::DelimiterType;
        }
        break;
        case DelimiterType:
        {
            delimitSymbol->m_tokenType = tmpString.ToInt();
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

    LexerState XenonScriptAssemblerMachine::DetermineLexerState(LexerState lexerState, char character, unsigned int index, bool& isShouldAddCharacter, bool& isTokenDone) const
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
            printf("Undefined State\n");
            return nullptr;
            break;

        }

        return lexerState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::TokenError(LexerState state, char character, unsigned int index) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d", character, currentLexerState, index);
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
                if (token->m_character == m_delimiterList[i]->m_symbol)
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