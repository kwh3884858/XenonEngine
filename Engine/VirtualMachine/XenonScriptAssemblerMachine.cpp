#include "XenonScriptAssemblerMachine.h"

#include "Algorithms/StreamingVector.h"

#include <stdio.h>  // printf

namespace XenonEnigne
{
    using Algorithm::StreamingVector;

    XenonScriptAssemblerMachine::XenonScriptAssemblerMachine()
    {
    }

    XenonScriptAssemblerMachine::~XenonScriptAssemblerMachine()
    {
    }

    bool XenonScriptAssemblerMachine::InitializeInstructionList(const XenonFile * const xenonFile)
    {
        int index = 0;
        bool isDone = false;
        bool isShouldAdd = true;
        InstructionState currentState = InstructionState::InstructionStateMnomonic;
        String tmpString;

        int tokenOpAmount = 0;
        int currentTokenOpCount = 0;

        int instructionLineForDebug = 1;

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
                    InstructionError(currentState, currentChar, index, instructionLineForDebug);
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
                    InstructionError(currentState, currentChar, index, instructionLineForDebug);
                    return false;
                }
            }
            break;
            case InstructionStateOpCount:
            {
                if (IsCharNumeric(currentChar))
                {
                }
                else if (IsNewLine(currentChar))
                {
                    isShouldAdd = false;
                    isDone = true;
                    instructionLineForDebug++;
                }
                else
                {
                    InstructionError(currentState, currentChar, index, instructionLineForDebug);
                    return false;
                }
            }
            break;
            case InstructionStateOpTypeList:
                if (IsCharIdent(currentChar))
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
                    instructionLineForDebug++;
                }
                else
                {
                    InstructionError(currentState, currentChar, index, instructionLineForDebug);
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
                    m_instructionLookupList.Add(instrction);
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
                            instrction->m_opFlags.Add(OP_FLAG_TYPE_NONE);
                        }
                        currentState = InstructionState::InstructionStateOpTypeList;
                    }
                    else
                    {
                        currentState = InstructionState::InstructionStateMnomonic;
                    }
                }
                break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionStateOpTypeList:
                {
                    if (currentTokenOpCount >= tokenOpAmount)
                    {
                        currentState = InstructionState::InstructionStateStart;
                        //Current Token Op is over max Op Vector Capacity.
                        currentTokenOpCount--;
                    }

                    int typeFlag = 0;
                    bool result = StringToType(keyWordString, tmpString, typeFlag);
                    if (!result)
                    {
                        InstructionError(InstructionState::InstructionStateOpTypeList, currentChar, index, instructionLineForDebug);
                        return false;
                    }
                    assert(result == true);
                    assert(typeFlag != 0);
                    OpBitfiledFlag currentFlag = instrction->m_opFlags[currentTokenOpCount];
                    currentFlag |= (typeFlag << 1);
                    instrction->m_opFlags[currentTokenOpCount] = currentFlag;
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

    bool XenonScriptAssemblerMachine::InitializeDelimiterList(const XenonFile*const xenonFile)
    {
        int index = 0;
        bool isDone = false;
        bool isShouldAdd = true;
        DelimiterSymbolState currentState = DelimiterSymbolState::Symbol;
        String tmpString;

        DelimiterSymbol* delimiterSymbol = nullptr;

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

        return true;
    }

    void XenonScriptAssemblerMachine::Compiler(const XenonFile* const xenonFile)
    {
        TokenVector* tokens = nullptr;
        tokens = Lexer(xenonFile);
        assert(tokens != nullptr);
        
        bool result = false;
        result = Parsing(tokens);
        assert(result == true);

        delete tokens;
        tokens = nullptr;
    }

    void XenonScriptAssemblerMachine::BuildXEX(XenonFile*const xenonFile) const
    {
        PrintAssemblerState(xenonFile);

        StreamingVector<char> executeStream;
        executeStream.Add(&m_scriptHeader.m_globalDataSize, sizeof(m_scriptHeader.m_globalDataSize));
        executeStream.Add(&m_scriptHeader.m_mainFunctionEntryIndex, sizeof(m_scriptHeader.m_mainFunctionEntryIndex));

        executeStream.Add(&(m_instructionList.Count()), sizeof(m_instructionList.Count()));
        for (int index = 0; index < m_instructionList.Count(); index++)
        {
            executeStream.Add(&m_instructionList[index]->m_opCode, sizeof(m_instructionList[index]->m_opCode));
            unsigned int opCount = m_instructionList[index]->m_opCount;
            for (int opIndex = 0; opIndex < opCount; opIndex++)
            {
                executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_type, sizeof(m_instructionList[index]->m_ops[opIndex]->m_type));
                if (m_instructionList[index]->m_ops[opIndex]->m_type == InstructionOpType::InstructionOpTypeFloatLiteral)
                {
                    executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_floatLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_floatLiteral));
                }
                else
                {
                    executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_interalLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_interalLiteral));
                }
            }
        }

        //executeStream.Add(&m_symbolTable.Count(), sizeof(m_symbolTable.Count()));
        //for (int index = 0; index < m_symbolTable.Count(); index++)
        //{
        //    executeStream.Add(&m_symbolTable[index]->m_variableType, sizeof(m_symbolTable[index]->m_variableType));
        //    assert(m_symbolTable[index]->m_size > 0);
        //    executeStream.Add(&m_symbolTable[index]->m_size, sizeof(m_symbolTable[index]->m_size));
        //    executeStream.Add(&m_symbolTable[index]->m_stackIndex, sizeof(m_symbolTable[index]->m_stackIndex));
        //    executeStream.Add(&m_symbolTable[index]->m_functionIndex, sizeof(m_symbolTable[index]->m_functionIndex));
        //    executeStream.Add(&m_symbolTable[index]->m_symbolToken->m_)
        //}

        executeStream.Add(&m_functionTable.Count(), sizeof(m_functionTable.Count()));
        for (int index = 0; index < m_functionTable.Count(); index++)
        {
            executeStream.Add(&m_functionTable[index]->m_functionIndex, sizeof(m_functionTable[index]->m_functionIndex));
            executeStream.Add(&m_functionTable[index]->m_entryPoint, sizeof(m_functionTable[index]->m_entryPoint));
            executeStream.Add(&m_functionTable[index]->m_localStackSize, sizeof(m_functionTable[index]->m_localStackSize));
            executeStream.Add(&m_functionTable[index]->m_parameterCount, sizeof(m_functionTable[index]->m_parameterCount));
        }

        executeStream.Add(&m_labelTable.Count(), sizeof(m_labelTable.Count()));
        for (int index= 0;index<m_labelTable.Count(); index++)
        {
            executeStream.Add(m_labelTable[index]->m_instructionStreamIndex, sizeof(m_labelTable[index]->m_instructionStreamIndex));

        }

        executeStream.Add(&m_stringTable.Count(), sizeof(m_stringTable.Count()));
        for (int index = 0; index < m_stringTable.Count(); index++)
        {
            executeStream.Add(&(m_stringTable[index].Count()), sizeof(m_stringTable[index].Count()));
            executeStream.Add(m_stringTable[index].Beign(), sizeof(char), m_stringTable[index].Count());
        }

        executeStream.Add(&m_hostAPITable.Count(), sizeof(m_hostAPITable.Count()));
        for (int index = 0; index < m_hostAPITable; index++)
        {
            executeStream.Add(&(m_hostAPITable[index].Count()), sizeof(m_hostAPITable[index].Count()));
            executeStream.Add(m_hostAPITable[index].Beign(), sizeof(char), m_hostAPITable[index].Count());
        }

        int pos = xenonFile->m_fileName.Find(".xea");
        String outputFileName = xenonFile->m_fileName.Substring(0, pos);
        outputFileName.Append(".xex");

        FileManager::Get().WriteFile(outputFileName, &executeStream);
    }

    void XenonScriptAssemblerMachine::InstructionError(InstructionState state, char character, int index, int lineISize) const
    { 
        printf("Fetal Error: Construct Instruction Error\n Character '%c' From State %d is undefined\n In the index %d in Line %d\n", character, state, index, lineISize);

    }

    void XenonScriptAssemblerMachine::UpdateCharacter(char currentChar, bool& isShouldAdd, bool& isDone) const
    {
/*        if (IsNewLine(currentChar) || currentChar == SymbolComma)
        {
            isShouldAdd = false;
            isDone = true;
        }
        else*/ if (IsCharWhitespace(currentChar))
        {
            isShouldAdd = false;
            isDone = true;
        }
        else
        {
            isShouldAdd = true;
        }
    }

    XenonEnigne::XenonScriptAssemblerMachine::DelimiterSymbolState XenonScriptAssemblerMachine::CreateDelimiterList(DelimiterSymbolState currentState, const String& tmpString, DelimiterSymbol*& delimiterSymbol)
    {
        switch (currentState)
        {
        case Symbol:
        {
            delimiterSymbol = new DelimiterSymbol;
            delimiterSymbol->m_symbol = tmpString.ToChar();
            currentState = DelimiterSymbolState::DelimiterType;
        }
        break;
        case DelimiterType:
        {
            delimiterSymbol->m_tokenType = static_cast<DelimiterWord>(tmpString.ToInt());
            currentState = DelimiterSymbolState::Symbol;
            m_delimiterList.Add(delimiterSymbol);
        }
        break;
        default:
            break;
        }
        return currentState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::GetNextToken(const XenonFile * const xenonFile, int& refCurrentIndex, Token* const token) const
    {
        char currentCharacter = '\0';
        bool isShouldAddCharacter = true;
        LexerState currentLexerState = LexerState::LexerStateStart;
        bool isTokenDone = false;
        int lineCountForDebug = 1;

        while (!isTokenDone) {
            isShouldAddCharacter = true;

            //Get Next Character
            if (refCurrentIndex >= xenonFile->m_content.Count())
            {
                if (token->m_character.Count() != 0)
                {
                    DetermineTokenType(token, currentLexerState);
                    return currentLexerState;
                }
                return LexerState::LexerStateDone;
            }
            currentCharacter = xenonFile->m_content[refCurrentIndex];

            currentLexerState = DetermineLexerState(currentLexerState, currentCharacter, refCurrentIndex, isShouldAddCharacter, isTokenDone, lineCountForDebug);

            refCurrentIndex++;

            if (isShouldAddCharacter)
            {
                token->m_character.Add(currentCharacter);
            }

            if (isTokenDone)
            {
                DetermineTokenType(token, currentLexerState);
                if (currentLexerState != LexerState::LexerStateStringEnd)
                {
                    refCurrentIndex--;
                }
                return currentLexerState;
            }
        }

        return currentLexerState;
    }

    XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::DetermineLexerState(LexerState lexerState, char character, unsigned int index, bool& isShouldAddCharacter, bool& isTokenDone, int& refLineCountForDebug) const
    {
        switch (lexerState)
        {
        case LexerState::LexerStateStart:
        {
            if (IsNewLine(character))
            {
                refLineCountForDebug++;
            }

            if (IsCharWhitespace(character))
            {
                isShouldAddCharacter = false;
            }
            else if (character == SymbolSemicolon)
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
                return TokenError(lexerState, character, index, refLineCountForDebug);
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
            else if (IsCharDelimiter(character))
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
                //lexerState = LexerState::LexerStateDelimiter;
            }
            else
            {
                return TokenError(lexerState, character, index, refLineCountForDebug);
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
                return TokenError(lexerState, character, index, refLineCountForDebug);
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
            else if (IsCharDelimiter(character))
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
                //lexerState = LexerState::LexerStateDelimiter;
            }
            else
            {
                return TokenError(lexerState, character, index, refLineCountForDebug);
            }
        }
        break;
        case LexerStateString:
        {
            if (character == SymbolQuote)
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
                lexerState = LexerState::LexerStateStringEnd;
            }
            else if(character == SymbolBackslash)
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
            TokenError(lexerState, character, index, refLineCountForDebug);
            return LexerState::LexerStateError;
        }


        }

        return lexerState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::TokenError(LexerState state, char character, unsigned int index, int lineCountForDebug) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d and line %d", character, state, index, lineCountForDebug);
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
            for (int i = 0; i < m_instructionLookupList.Count(); i++)
            {
                if (token->m_character == m_instructionLookupList[i]->m_mnemonic)
                {
                    token->m_tokenType = TokenType::Keyword;
                    token->m_keyword = m_instructionLookupList[i]->m_opType;
                    break;
                }
            }
            break;
        case LexerStateString:
            token->m_tokenType = TokenType::StringEntity;
            break;
        case LexerStateStringEnd:
            token->m_tokenType = TokenType::StringEntity;
            break;
        case LexerStateEscape:
            break;
        case LexerStateIntegral:
            token->m_tokenType = TokenType::IntergalIiteral;
            break;
        case LexerStateFloat:
            token->m_tokenType = TokenType::FloatIiteral;
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

    XenonScriptAssemblerMachine::TokenVector* XenonScriptAssemblerMachine::Lexer(const XenonFile * const xenonFile) const
    {
        int currentIndex = 0;
        TokenVector* tokenVector = new TokenVector;
        while (currentIndex <= xenonFile->m_content.Count()) {
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
        assert(true == false);
    }

    bool XenonScriptAssemblerMachine::Parsing(TokenVector* const tokenVector)
    {
        bool result = false;
        result = BuildSymbolAndFunctionAndLabelTable(tokenVector);
        assert(result == true);

        Vector<Instruction*> instructionStream;
        result =  CreateInstructionList(tokenVector, instructionStream);
        assert(result == true);

        return true;
    }

    bool XenonScriptAssemblerMachine::BuildSymbolAndFunctionAndLabelTable(TokenVector* const tokenVector)
    {
        FunctionElement* currentFunction = nullptr;
        unsigned int instructionStreamCount = 0;

        for (int index = 0; index < tokenVector->Count(); index++)
        {
            Token * currentToken = (*tokenVector)[index];
            switch (currentToken->m_tokenType)
            {
            case IntergalIiteral:
            {

            }
            break;
            case FloatIiteral:
            {

            }
            break;
            case Identifier:
            {
                // Line Label
                Token* token = PeekNextToken(*tokenVector, index);
                if (!token || token->m_tokenType != TokenType::Delimiter)
                {
                    continue;
                }

                if (token->m_delimiter == DelimiterWord::Colon)
                {
                    Token* token = MoveToNextToken(*tokenVector, index);

                    LabelElement* const label = new LabelElement;
                    label->m_token = currentToken;
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
                case INT:
                {
                    bool result = CreateSymbol(tokenVector, currentToken, InstructionOpType::InstructionOpTypeInteralLiteral, currentFunction, index, m_scriptHeader.m_globalDataSize);
                    if (!result)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }
                }
                break;
                case FLOAT:
                {
                    bool result = CreateSymbol(tokenVector, currentToken, InstructionOpType::InstructionOpTypeFloatLiteral, currentFunction, index, m_scriptHeader.m_globalDataSize);
                    if (!result)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }
                }
                break;
                case FUNC:
                {
                    if (currentFunction)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    Token* token = MoveToNextToken(*tokenVector, index);
                    if (!token || token->m_tokenType != TokenType::Identifier)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    token->m_tokenType = TokenType::Function;

                    currentFunction = new FunctionElement;
                    currentFunction->m_functionToken = token;
                    currentFunction->m_functionIndex = m_functionTable.Count();
                    currentFunction->m_entryPoint = instructionStreamCount;

                    m_functionTable.Add(currentFunction);

                    if (token->m_character == Main_Function_Name)
                    {
                        m_scriptHeader.m_mainFunctionEntryIndex = currentFunction->m_functionIndex;
                    }

                    token = MoveToNextToken(*tokenVector, index);
                    if (!token || token->m_tokenType != TokenType::Delimiter)
                    {
                        if (!token || token->m_delimiter != DelimiterWord::OpenBrace)
                        {
                            BuildTableError(currentToken, index);
                            return false;
                        }
                    }

                    instructionStreamCount++;
                }
                break;
                case PARAM:
                {
                    if (!currentFunction)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    if (currentFunction->m_functionToken->m_character == Main_Function_Name)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    Token* token = MoveToNextToken(*tokenVector, index);
                    if (!token || token->m_tokenType != TokenType::Identifier)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    currentFunction->m_parameterCount++;
                }
                break;
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
                case CALL:
                    break;
                case RET:
                    break;
                case CALLHOST:
                {

                }
                    break;
                case PAUSE:
                    break;
                case EXIT:
                    break;
                }
            }
            break;
            case Delimiter:
            {
                if (currentToken->m_delimiter == DelimiterWord::CloseBrace)
                {
                    if (currentFunction)
                    {
                        currentFunction = nullptr;
                    }
                    else
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }
                }
            }
            break;
            case None:
                break;
            case HostAPI:
                break;
            case Register:
                break;
            case TokenTypeCount:
                break;
            default:
            {
                if (!currentFunction)
                {
                    BuildTableError(currentToken, index);
                    return false;
                }

                instructionStreamCount++;
            }
            break;
            }
        }
        assert(currentFunction == nullptr);
        return true;
    }

    void XenonScriptAssemblerMachine::BuildTableError(const Token* const token, int index)
    {
        char errorToken[64];
        token->m_character.CString(errorToken);
        printf("Fetal Error: Parsing Error\n Character %c In the index %d \n", errorToken, index);
    }

    bool XenonScriptAssemblerMachine::CreateInstructionList(TokenVector* const tokenVector, const Vector<Instruction *>& instructionStream)
    {
        FunctionElement* currentFunction = nullptr;
        int currentFunctionParamCount = 0;
        Instruction* currentInstrction = nullptr;
        InstructionLookup* currentInstructionLookup;

        for (int index = 0; index < tokenVector->Count(); index++)
        {
            Token * currentToken = (*tokenVector)[index];
            switch (currentToken->m_tokenType)
            {
            case None:
                break;
            case IntergalIiteral:
            {
            }
            break;
            case FloatIiteral:
                break;
            case StringEntity:
                break;
            case Label:
                break;
            case Function:
            {
                //Token *token = MoveToNextToken(*tokenVector, index);
                FunctionElement*const functionElement = GetFunctionByName(currentToken->m_character);
                if (functionElement)
                {
                    currentFunction = functionElement;
                    currentFunctionParamCount = 0;
                }
                else
                {
                    return false;
                }
            }

            break;
            case TokenType::Keyword:
            {
                switch (currentToken->m_keyword)
                {
                case KeyWord::PARAM:
                {
                    currentToken = MoveToNextToken(*tokenVector, index);
                    assert(currentToken->m_tokenType == TokenType::Keyword);

                    InstructionOpType variable = (currentToken->m_keyword == KeyWord::INT) ?
                        InstructionOpType::InstructionOpTypeInteralLiteral : InstructionOpType::InstructionOpTypeFloatLiteral;

                    currentToken = MoveToNextToken(*tokenVector, index);
                    assert(currentToken->m_tokenType == TokenType::Identifier);

                    currentFunctionParamCount++;
                    assert(currentFunctionParamCount <= currentFunction->m_parameterCount);
                    int stackIndex = -(Local_Stack_Start_Index + currentFunction->m_localStackSize + currentFunctionParamCount);

                    assert(currentFunction != nullptr);
                    if (currentFunction)
                    {
                        SymbolElement* symbol = new SymbolElement;
                        symbol->m_variableType = variable;
                        symbol->m_symbolToken = currentToken;
                        symbol->m_size = 1;
                        symbol->m_stackIndex = stackIndex;
                        symbol->m_functionIndex = currentFunction->m_functionIndex;

                        m_symbolTable.Add(symbol);
                    }
                }
                break;
                case KeyWord::FUNC:
                    continue;
                }

                currentInstrction = new Instruction;
                currentInstructionLookup = GetInstructionByKeyword(currentToken->m_keyword);

                currentInstrction->m_opCode = currentToken->m_keyword;
                currentInstrction->m_opCount = currentInstructionLookup->m_opCount;
                for (int i = 0; i < currentInstrction->m_opCount; i++)
                {
                    currentInstrction->m_ops.Add(new InstructionOp);
                }

                m_instructionList.Add(currentInstrction);

                for (int parameterIndex = 0; parameterIndex < currentInstrction->m_opCount; parameterIndex++)
                {
                    currentToken = MoveToNextToken(*tokenVector, index);

                    if (!((currentInstructionLookup->m_opFlags[parameterIndex] & (currentToken->m_tokenType << 1)) > 0))
                    {
                        CreateInstructionListError(currentToken, index);
                        return false;
                    }

                    switch (currentToken->m_tokenType)
                    {
                    case None:
                        break;
                    case IntergalIiteral:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeInteralLiteral;
                        currentInstrction->m_ops[parameterIndex]->m_interalLiteral = currentToken->m_character.ToInt();
                    }
                    break;
                    case FloatIiteral:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeFloatLiteral;
                        currentInstrction->m_ops[parameterIndex]->m_floatLiteral = currentToken->m_character.ToFloat();
                    }
                    break;
                    case StringEntity:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeStringIndex;
                        int indexOfString = m_stringTable.IndexOf(currentToken->m_character);
                        if (indexOfString != -1)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_stringTableIndex = m_stringTable.Count();
                            m_stringTable.Add(currentToken->m_character);
                        }
                        else
                        {
                            currentInstrction->m_ops[parameterIndex]->m_stringTableIndex = indexOfString;
                        }
                    }
                    break;
                    case TokenType::Register:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeRegister;
                        currentInstrction->m_ops[parameterIndex]->m_floatLiteral = 0;
                    }
                    break;
                    case TokenType::Identifier:
                    {
                        String identifier = currentToken->m_character;
                        SymbolElement* const symbolElement = GetSymbolByName(identifier);
                        FunctionElement* const functionElement = GetFunctionByName(identifier);
                        LabelElement* const labelElement = GetLabelByName(identifier);

                        if (symbolElement != nullptr)
                        {
                            if (symbolElement->m_size > 1)
                            {
                                currentToken = MoveToNextToken(*tokenVector, index);
                                assert(currentToken->m_delimiter == DelimiterWord::OpenBracket);
                                currentToken = MoveToNextToken(*tokenVector, index);
                                assert(currentToken->m_tokenType == TokenType::IntergalIiteral);

                                if (currentToken->m_tokenType == TokenType::IntergalIiteral)
                                {
                                    currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeAbsoluteStackIndex;
                                    int arrayIndex = currentToken->m_character.ToInt();
                                    currentInstrction->m_ops[parameterIndex]->m_stackIndex = symbolElement->m_stackIndex + arrayIndex;
                                }
                                else if (currentToken->m_tokenType == TokenType::Identifier)
                                {
                                    currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeRelativeStackIndex;
                                    SymbolElement* const indexSymbol = GetSymbolByName(currentToken->m_character);
                                    assert(indexSymbol != nullptr);
                                    currentInstrction->m_ops[parameterIndex]->m_stackIndex = symbolElement->m_stackIndex;
                                    currentInstrction->m_ops[parameterIndex]->m_offsetIndex = indexSymbol->m_stackIndex;
                                }
                                else
                                {
                                    CreateInstructionListError(currentToken, index);
                                    return false;
                                }
                            }
                            else
                            {
                                currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeAbsoluteStackIndex;
                                currentInstrction->m_ops[parameterIndex]->m_stackIndex = symbolElement->m_stackIndex;
                            }
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] &  TokenType::Function <<1) > 0 &&
                            functionElement != nullptr)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeFunctionIndex;
                            currentInstrction->m_ops[parameterIndex]->m_stackIndex = functionElement->m_functionIndex;
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] &  TokenType::Label << 1) > 0 &&
                            labelElement != nullptr)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeInstructionIndex;
                            currentInstrction->m_ops[parameterIndex]->m_stackIndex = labelElement->m_instructionStreamIndex;
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] &  TokenType::HostAPI << 1) > 0)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeHostAPICallIndex;
                            int indexOfHostAPI = m_hostAPITable.IndexOf(currentToken->m_character);
                            if (indexOfHostAPI != -1)
                            {
                                currentInstrction->m_ops[parameterIndex]->m_stringTableIndex = m_hostAPITable.Count();
                                m_hostAPITable.Add(currentToken->m_character);
                            }
                            else
                            {
                                currentInstrction->m_ops[parameterIndex]->m_stringTableIndex = indexOfHostAPI;
                            }
                        }
                        else
                        {
                            CreateInstructionListError(currentToken, index);
                            return false;
                        }
                    }
                    break;
                    case Label:
                        break;
                    case Function:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpTypeFunctionIndex;
                        FunctionElement* const functionElement = GetFunctionByName(currentToken->m_character);
                        if (functionElement)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_funcIndex = functionElement->m_functionIndex;
                        }
                        else
                        {
                            CreateInstructionListError(currentToken, index);
                            return false;
                        }
                    }
                    break;
                    case HostAPI:
                        break;
                    case Delimiter:
                        break;
                    case TokenTypeCount:
                        break;
                    default:
                        break;

                    }
                }
            }
            break;
            case HostAPI:
                break;
            case Register:
                break;
            case Delimiter:
            {
                switch (currentToken->m_delimiter)
                {
                case Colon:
                    break;
                case SemiColon:
                    break;
                case OpenBracket:
                    break;
                case CloseBracket:
                    break;
                case Comma:
                    break;
                case OpenBrace:
                    break;
                case CloseBrace:
                {
                    currentInstrction = new Instruction;
                    currentInstrction->m_opCount = currentInstructionLookup->m_opCount;

                    if (currentFunction->m_functionIndex == m_scriptHeader.m_mainFunctionEntryIndex)
                    {
                        currentInstrction->m_opCode = KeyWord::EXIT;
                        currentInstrction->m_opCount = 1;
                        InstructionOp* const op = new InstructionOp();
                        op->m_type = InstructionOpType::InstructionOpTypeInteralLiteral;
                        op->m_interalLiteral = 0;
                        currentInstrction->m_ops.Add(op);
                    }
                    else
                    {
                        currentInstrction->m_opCode = KeyWord::EXIT;
                        currentInstrction->m_opCount = 0;
                    }
                    currentFunction = nullptr;
                    m_instructionList.Add(currentInstrction);
                }
                break;
                default:
                    break;
                }
            }
            break;
            case TokenTypeCount:
                break;
            default:
                break;
            }
        }
        return true;
    }

    void XenonScriptAssemblerMachine::CreateInstructionListError(const Token* const token, int index)
    {
        char errorToken[64];
        token->m_character.CString(errorToken);
        printf("Fetal Error: Parsing Create Instruction List Error\n Character %c In the index %d \n", errorToken, index);
    }

    void XenonScriptAssemblerMachine::PrintAssemblerState(XenonFile*const xenonFile) const
{
        int variableCount = 0;
        int arrayCount = 0;
        int globalCount =0;
        for (int i = 0;i < m_symbolTable.Count(); i++)
        {
           if (m_symbolTable[i]->m_size >1)
           {
               arrayCount++;
           }
           else {
               variableCount++;
           }
           if (m_symbolTable[i]->m_stackIndex >= 0)
           {
               ++globalCount;
           }
        }

        // Print out final calculations
        char filePaht[MAX_PATH];
        xenonFile->m_fileName.CString(filePaht);

        printf("%Created successfully!\n\n");
        printf("File Path: %s\n", filePaht);
        printf("Global Data Size: %d\n", m_scriptHeader.m_globalDataSize);

        printf("\n");
        printf("Instructions Assembled: %d\n", m_instructionList.Count());
        printf("             Variables: %d\n", variableCount);
        printf("                Arrays: %d\n", arrayCount);
        printf("               Globals: %d\n", globalCount);
        printf("       String Literals: %d\n", m_stringTable.Count());
        printf("                Labels: %d\n", m_labelTable.Count());
        printf("        Host API Calls: %d\n", m_hostAPITable.Count());
        printf("             Functions: %d\n", m_functionTable.Count());

        printf("      _Main () Present: ");
        if (m_scriptHeader.m_mainFunctionEntryIndex != -1)
            printf("Yes (Index %d)\n", m_scriptHeader.m_mainFunctionEntryIndex);
        else
            printf("No\n");
    }

    bool XenonScriptAssemblerMachine::CreateSymbol(TokenVector* const tokenVector, Token* currentToken, InstructionOpType tokenType, FunctionElement* const functionElement, int& refIndex, unsigned int& refGlobalStackSize)
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
            if (!token || token->m_tokenType != TokenType::IntergalIiteral)
            {
                return false;
            }
            size = token->m_character.ToInt();
            token = (*tokenVector)[++refIndex];
            if (!token || token->m_delimiter != DelimiterWord::OpenBracket)
            {
                return false;
            }
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
        symbol->m_variableType = tokenType;
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

        return true;
    }

    bool XenonScriptAssemblerMachine::IsNewLine(char character) const
    {
        if (character == '\n' || character == '\r')
            return true;
        else
            return false;
    }

    bool XenonScriptAssemblerMachine::IsCharWhitespace(char character) const
    {
        // Return true if the character is a space or tab.
        if (IsNewLine(character))
            return true;
        else if (character == ' ' || character == '\t' )
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

    XenonEnigne::XenonScriptAssemblerMachine::Token* XenonScriptAssemblerMachine::MoveToNextToken(const TokenVector& tokenVector, int& index) const
    {
        assert(index + 1 < tokenVector.Count());
        return tokenVector[++index];
    }

    XenonScriptAssemblerMachine::Token* XenonScriptAssemblerMachine::PeekNextToken(const TokenVector& tokenVector, int& index) const
    {
        assert(index + 1 < tokenVector.Count());
        return tokenVector[index+1];
    }

    XenonEnigne::XenonScriptAssemblerMachine::SymbolElement*const XenonScriptAssemblerMachine::GetSymbolByName(const String& symbolName) const
    {
        for (int i = 0; i < m_symbolTable.Count(); i++)
        {
            if (m_symbolTable[i]->m_symbolToken->m_character == symbolName)
            {
                return m_symbolTable[i];
            }
        }
        return nullptr;
    }

    XenonScriptAssemblerMachine::FunctionElement*const XenonScriptAssemblerMachine::GetFunctionByName(const String& functionName) const
    {
        for (int i = 0; i < m_functionTable.Count(); i++)
        {
            if (m_functionTable[i]->m_functionToken->m_character == functionName)
            {
                return m_functionTable[i];
            }
        }
        return nullptr;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LabelElement*const XenonScriptAssemblerMachine::GetLabelByName(const String& labelName)
    {
        for (int i = 0; i < m_labelTable.Count(); i++)
        {
            if (m_labelTable[i]->m_token->m_character == labelName)
            {
                return m_labelTable[i];
            }
        }
        return nullptr;
    }

    XenonEnigne::XenonScriptAssemblerMachine::InstructionLookup*const XenonScriptAssemblerMachine::GetInstructionByKeyword(const KeyWord& keyword) const
    {
        for (int i = 0; i < m_instructionLookupList.Count(); i++)
        {
            if (m_instructionLookupList[i]->m_opType == keyword)
            {
                return m_instructionLookupList[i];
            }
        }
        return nullptr;
    }

    const XenonEnigne::TypeString<int> XenonScriptAssemblerMachine::keyWordString[keywordStringCount] =
    {
        {"int",  TokenType::IntergalIiteral},
        {"float", TokenType::FloatIiteral},
        {"string",  TokenType::StringEntity},
        {"identifier",  TokenType::Identifier},
        {"label",  TokenType::Label},
        {"function",  TokenType::Function},
        {"hostAPI",  TokenType::HostAPI},
        {"register",   TokenType::Register}
    };


}