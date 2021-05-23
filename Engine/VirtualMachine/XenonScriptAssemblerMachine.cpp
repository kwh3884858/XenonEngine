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

        InstructionLookup* instrction;

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
                    instrction = new InstructionLookup;
                    instrction->m_mnemonic = tmpString;
                    currentState = InstructionState::InstructionStateOpType;
                }
                break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateOpType:
                {
                    instrction->m_opType = static_cast<KeyWord>(tmpString.ToInt());
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
                    int typeFlag = 0;
                    bool result = StringToType(keyWordString, tmpString, typeFlag);
                    assert(result == true);
                    assert(typeFlag != 0);
                    instrction->op[currentTokenOpCount] &= 1 << typeFlag;
                    if (currentTokenOpCount >= tokenOpAmount)
                    {
                        currentState = InstructionState::InstructionStateStart;
                    }
                }
                break;
                default:
                    break;
                }

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

    XenonEnigne::XenonScriptAssemblerMachine::InstructionState XenonScriptAssemblerMachine::CreateInstructionList(InstructionState currentState, const String& tmpString, InstructionLookup*const instruction, int& tokenOpAmount, int& currentTokenopCount)
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
            delimitSymbol->m_tokenType = static_cast<DelimiterWord>(tmpString.ToInt());
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
            token->m_tokenType = TokenType::Delimiter;
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

    bool XenonScriptAssemblerMachine::Parsing(TokenVector* const tokenVector)
    {
        unsigned int globalStackSize = 0;

        FunctionElement* currentFunction = nullptr;
        unsigned int instructionStreamCount = 0;

        ScriptHeader scriptHeader;
        for (unsigned int index = 0; index < tokenVector->Count(); index++)
        {
            Token * currentToken = (*tokenVector)[index];
            switch (currentToken->m_tokenType)
            {
            case Intergal:
            {
                bool result = CreateSymbol(tokenVector, currentToken, TokenType::Intergal, currentFunction, index, globalStackSize);
                if (!result)
                {
                    return false;
                }
            }
            break;
            case Float:
            {
                bool result = CreateSymbol(tokenVector, currentToken, TokenType::Float, currentFunction, index, globalStackSize);
                if (!result)
                {
                    return false;
                }
            }
            break;
            case Identifier:
            {
                // Line Label
                Token* token = (*tokenVector)[index - 1];
                if (!token || token->m_tokenType != TokenType::Delimiter)
                {
                    return false;
                }

                if (token->m_delimiter == DelimiterWord::Colon)
                {
                    LabelElement* const label = new LabelElement;
                    label->token = currentToken;
                    label->m_instructionStreamIndex = instructionStreamCount;
                    label->m_currentFunction = currentFunction;
                    m_labelTable.Add(label);
                }
            }
            break;
            case StringEntity:
                break;
            case TokenType::Label:
                break;
            case TokenType::Function:
                break;
            case Keyword:
            {
                switch (currentToken->m_keyword)
                {
                case FUNC:
                {
                    if (currentFunction)
                    {
                        return false;
                    }

                    Token* token = (*tokenVector)[++index];
                    if (!token || token->m_tokenType != TokenType::Identifier)
                    {
                        return false;
                    }

                    currentFunction = new FunctionElement;
                    currentFunction->m_functionToken = token;
                    currentFunction->m_functionIndex = m_functionTable.Count();
                    currentFunction->m_entryPoint = instructionStreamCount;

                    m_functionTable.Add(currentFunction);

                    token = (*tokenVector)[++index];
                    if (!token || token->m_tokenType != TokenType::Delimiter)
                    {
                        if (!token || token->m_delimiter != DelimiterWord::OpenBrace)
                        {
                            return false;
                        }
                    }

                    if (token->m_character == Main_Function_Name)
                    {
                        scriptHeader.m_isMainEntryExist = true;
                        scriptHeader.m_mainFunctionEntryIndex = currentFunction->m_functionIndex;
                    }

                    instructionStreamCount++;
                }
                break;
                case PARAM:
                {
                    if (!currentFunction)
                    {
                        return false;
                    }

                    if (currentFunction->m_functionToken->m_character == Main_Function_Name)
                    {
                        return false;
                    }

                    Token* token = (*tokenVector)[++index];
                    if (!token || token->m_tokenType != TokenType::Identifier)
                    {
                        return false;
                    }

                    currentFunction->m_parameterCount++;
                }
                break;
                default:
                {
                    if (!currentFunction)
                    {
                        return false;
                    }

                    instructionStreamCount++;
                }
                break;
                }
            }
            break;
            case Delimiter:
            {
                if (currentToken->m_delimiter == DelimiterWord::CloseBracket)
                {
                    if (currentFunction)
                    {
                        currentFunction = nullptr;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            break;
            }
        }

        assert(currentFunction == nullptr);

        for (unsigned int index = 0; index < tokenVector->Count(); index++)
        {
            Token * currentToken = (*tokenVector)[index];
            switch (currentToken->m_tokenType)
            {
            case None:
                break;
            case Intergal:
                break;
            case Float:
                break;
            case StringEntity:
                break;
            case Identifier:
                break;
            case Label:
                break;
            case Function:
            {
                Token *token = MoveToNextToken(*tokenVector, index);
                FunctionElement*const function = GetFunctionByName(token->m_character);
                if (!function)
                {
                    return false;
                }
            }
                currentFunction = function;

                break;
            case HostAPI:
                break;
            case Register:
                break;
            case Keyword:
                switch (currentToken->m_keyword)
                {
                case MOV:
                    break;
                case ADD:
                    break;
                case SUB:
                    break;
                case MUL:
                    break;
                case DIV:
                    break;
                case MOD:
                    break;
                case EXP:
                    break;
                case NEG:
                    break;
                case INC:
                    break;
                case DEC:
                    break;
                case AND:
                    break;
                case OR:
                    break;
                case XOR:
                    break;
                case NOT:
                    break;
                case SHL:
                    break;
                case SHR:
                    break;
                case CONCAT:
                    break;
                case GETCHAR:
                    break;
                case SETCHAR:
                    break;
                case JMP:
                    break;
                case JE:
                    break;
                case JNE:
                    break;
                case JG:
                    break;
                case JL:
                    break;
                case JGE:
                    break;
                case JLE:
                    break;
                case PUSH:
                    break;
                case POP:
                    break;
                case FUNC:
                    break;
                case PARAM:
                    break;
                case CALL:
                    break;
                case RET:
                    break;
                case CALLHOS:
                    break;
                case PAUSE:
                    break;
                case EXIT:
                    break;
                default:
                    break;
                    
                }
                break;
            case Delimiter:
                break;
            case TokenTypeCount:
                break;
            default:
                break;
            }
        }

        delete tokenVector;
    }

    bool XenonScriptAssemblerMachine::CreateSymbol(TokenVector* const tokenVector, Token* currentToken, TokenType tokenType, FunctionElement* const functionElement, unsigned int& refIndex, unsigned int& refGlobalStackSize)
    {
        Token* token = (*tokenVector)[++refIndex];
        if (!token || token->m_tokenType != TokenType::Identifier)
        {
            return false;
        }

        int size = 1;
        token = (*tokenVector)[++refIndex];
        if (!token || token->m_tokenType == TokenType::Delimiter)
        {
            if (!token || token->m_delimiter != DelimiterWord::OpenBracket)
            {
                return false;
            }
            token = (*tokenVector)[++refIndex];
            if (!token || token->m_tokenType != TokenType::Intergal)
            {
                return false;
            }
            size = token->m_character.ToInt();
            token = (*tokenVector)[++refIndex];
            if (!token || token->m_delimiter != DelimiterWord::OpenBracket)
            {
                return false;
            }

            int stackIndex = 0;
            if (functionElement)
            {
                stackIndex = -(Local_Stack_Start_Index + functionElement->m_localStackSize);
            }
            else
            {
                stackIndex = refGlobalStackSize;
            }

            SymbolElement* symbol = new SymbolElement;
            symbol->m_variableType = TokenType::Intergal;
            symbol->m_symbolToken = currentToken;
            symbol->m_size = size;
            symbol->m_stackIndex = stackIndex;
            symbol->m_functionIndex = functionElement->m_functionIndex;

            m_symbolTable.Add(symbol);
            if (functionElement)
            {
                functionElement->m_localStackSize += size;
            }
            else
            {
                refGlobalStackSize += size;
            }
        }
        return true;
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

    XenonEnigne::XenonScriptAssemblerMachine::Token* XenonScriptAssemblerMachine::MoveToNextToken(const TokenVector& tokenVector, unsigned int& index) const
    {
        assert(index + 1 < tokenVector.Count());
        return tokenVector[index++];
    }

    XenonScriptAssemblerMachine::FunctionElement*const XenonScriptAssemblerMachine::GetFunctionByName(const String& functionName) const
    {
        for (int i = 0; i < m_functionTable.Count(); i++)
        {
            if (m_functionTable[i]->m_functionToken->m_character==functionName)
            {
                return m_functionTable[i];
            }
        }
        return nullptr;
    }
}