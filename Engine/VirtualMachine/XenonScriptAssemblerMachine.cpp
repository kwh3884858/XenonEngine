#include "XenonScriptAssemblerMachine.h"

#include "Algorithms/StreamingVector.h"
#include "Engine/FileManager/FileManager.h"
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
        InstructionState currentState = InstructionState::InstructionState_Mnomonic;
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
            case InstructionState_Start:
            {
                if (IsCharIdent(currentChar))
                {
                    currentState = InstructionState::InstructionState_Mnomonic;
                }
                else
                {
                    isShouldAdd = false;
                }
            }
            break;
            case InstructionState_Mnomonic:
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
            case InstructionState_OpType:
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
            case InstructionState_OpCount:
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
            case InstructionState_OpTypeList:
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
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionState_Start:
                    break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionState_Mnomonic:
                {
                    instrction = new InstructionLookup;
                    instrction->m_mnemonic = tmpString;
                    currentState = InstructionState::InstructionState_OpType;
                    m_instructionLookupList.Add(instrction);
                }
                break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionState_OpType:
                {
                    instrction->m_opType = static_cast<KeyWord>(tmpString.ToInt());
                    currentState = InstructionState::InstructionState_OpCount;
                }
                break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionState_OpCount:
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
                        currentState = InstructionState::InstructionState_OpTypeList;
                    }
                    else
                    {
                        currentState = InstructionState::InstructionState_Mnomonic;
                    }
                }
                break;
                case XenonEnigne::XenonScriptAssemblerMachine::InstructionState_OpTypeList:
                {
                    if (currentTokenOpCount >= tokenOpAmount)
                    {
                        currentState = InstructionState::InstructionState_Start;
                        //Current Token Op is over max Op Vector Capacity.
                        currentTokenOpCount--;
                    }

                    int typeFlag = 0;
                    bool result = StringToType(keyWordString, tmpString, typeFlag);
                    if (!result)
                    {
                        InstructionError(InstructionState::InstructionState_OpTypeList, currentChar, index, instructionLineForDebug);
                        return false;
                    }
                    assert(result == true);
                    assert(typeFlag != 0);
                    OpBitfiledFlag currentFlag = instrction->m_opFlags[currentTokenOpCount];
                    currentFlag |= (1 << typeFlag );
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
        DelimiterSymbolState currentState = DelimiterSymbolState::DelimiterSymbolState_Symbol;
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

        int count = m_instructionList.Count();
        executeStream.Add(&count, sizeof(count));
        for (int index = 0; index < m_instructionList.Count(); index++)
        {
            executeStream.Add(&m_instructionList[index]->m_opCode, sizeof(m_instructionList[index]->m_opCode));
            unsigned int opCount = m_instructionList[index]->m_opCount;
            executeStream.Add(&opCount, sizeof(opCount));
            for (int opIndex = 0; opIndex < opCount; opIndex++)
            {
                executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_type, sizeof(m_instructionList[index]->m_ops[opIndex]->m_type));
                //if (m_instructionList[index]->m_ops[opIndex]->m_type == InstructionOpType::InstructionOpType_FloatLiteral)
                //{
                //    executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_floatLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_floatLiteral));
                //}
                //else
                //{
                executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_integerLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_integerLiteral));
                //}
                executeStream.Add(&m_instructionList[index]->m_ops[opIndex]->m_offsetIndex, sizeof(m_instructionList[index]->m_ops[opIndex]->m_offsetIndex));
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
        {
            int count = m_functionTable.Count();
            executeStream.Add(&count, sizeof(count));
            for (int index = 0; index < m_functionTable.Count(); index++)
            {
                executeStream.Add(&m_functionTable[index]->m_functionIndex, sizeof(m_functionTable[index]->m_functionIndex));
                executeStream.Add(&m_functionTable[index]->m_entryPoint, sizeof(m_functionTable[index]->m_entryPoint));
                executeStream.Add(&m_functionTable[index]->m_localDataSize, sizeof(m_functionTable[index]->m_localDataSize));
                executeStream.Add(&m_functionTable[index]->m_parameterCount, sizeof(m_functionTable[index]->m_parameterCount));
            }
        }

        {
            int count = m_labelTable.Count();
            executeStream.Add(&count, sizeof(count));
            for (int index = 0; index < m_labelTable.Count(); index++)
            {
                executeStream.Add(&(m_labelTable[index]->m_instructionStreamIndex), sizeof(m_labelTable[index]->m_instructionStreamIndex));
            }
        }

        {
            int count = m_stringTable.Count();
            executeStream.Add(&count, sizeof(count));
            for (int index = 0; index < m_stringTable.Count(); index++)
            {
                int stringLength = m_stringTable[index].Count();
                executeStream.Add(&stringLength, sizeof(stringLength));
                executeStream.Add(m_stringTable[index].Beign(), sizeof(char), m_stringTable[index].Count());
            }
        }

        {
            int count = m_hostAPITable.Count();
            executeStream.Add(&count, sizeof(count));
            for (int index = 0; index < m_hostAPITable.Count(); index++)
            {
                int hostAPITableLength = m_hostAPITable[index].Count();
                executeStream.Add(&hostAPITableLength, sizeof(hostAPITableLength));
                executeStream.Add(m_hostAPITable[index].Beign(), sizeof(char), m_hostAPITable[index].Count());
            }
        }

        int pos = xenonFile->m_fileName.Find(".xea");
        String outputFileName = xenonFile->m_fileName.Substring(0, pos);
        outputFileName.Append(".xex");

        FileManager::Get().WriteFile(outputFileName, executeStream);
    }

    void XenonScriptAssemblerMachine::InstructionError(InstructionState state, char character, int index, int lineISize) const
    { 
        printf("Fetal Error: Construct Instruction Error\n Character '%c' From State %d is undefined\n In the index %d in Line %d\n", character, state, index, lineISize);

    }

    void XenonScriptAssemblerMachine::UpdateCharacter(char currentChar, bool& isShouldAdd, bool& isDone) const
    {
        if (IsCharWhitespace(currentChar))
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
        case DelimiterSymbolState_Symbol:
        {
            delimiterSymbol = new DelimiterSymbol;
            delimiterSymbol->m_symbol = tmpString.ToChar();
            currentState = DelimiterSymbolState::DelimiterSymbolState_DelimiterType;
        }
        break;
        case DelimiterSymbolState_DelimiterType:
        {
            delimiterSymbol->m_tokenType = static_cast<DelimiterWord>(tmpString.ToInt());
            currentState = DelimiterSymbolState::DelimiterSymbolState_Symbol;
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
        LexerState currentLexerState = LexerState::LexerState_Start;
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
                return LexerState::LexerState_Done;
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
                if (currentLexerState != LexerState::LexerState_StringEnd)
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
        case LexerState::LexerState_Start:
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
                lexerState = LexerState::LexerState_Comment;
            }
            else if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerState_Integral;
            }
            else if (IsCharIdent(character))
            {
                lexerState = LexerState::LexerState_Identifier;
            }
            else if (IsCharDelimiter(character))
            {
                lexerState = LexerState::LexerState_Delimiter;
            }
            else if (character == SymbolQuote) // "
            {
                isShouldAddCharacter = false;
                lexerState = LexerState::LexerState_String;
            }
            else
            {
                return TokenError(lexerState, character, index, refLineCountForDebug);
            }
        }
        break;
        case LexerState::LexerState_Integral:
        {
            if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerState_Integral;
            }
            else if (IsCharFullStop(character))
            {
                lexerState = LexerState::LexerState_Float;
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
                //lexerState = LexerState::LexerState_Delimiter;
            }
            else
            {
                return TokenError(lexerState, character, index, refLineCountForDebug);
            }
        }
        break;
        case LexerState::LexerState_Float:
        {
            if (IsCharNumeric(character))
            {
                lexerState = LexerState::LexerState_Float;
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
        case LexerState_Error:
            break;
        case LexerState_Done:
            break;
        case LexerState_Identifier:
        {
            if (IsCharIdent(character))
            {
                lexerState = LexerState::LexerState_Identifier;
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
                //lexerState = LexerState::LexerState_Delimiter;
            }
            else
            {
                return TokenError(lexerState, character, index, refLineCountForDebug);
            }
        }
        break;
        case LexerState_String:
        {
            if (character == SymbolQuote)
            {
                isShouldAddCharacter = false;
                isTokenDone = true;
                lexerState = LexerState::LexerState_StringEnd;
            }
            else if(character == SymbolBackslash)
            {
                isShouldAddCharacter = false;
                lexerState = LexerState::LexerState_Escape;
            }
        }
        break;
        case LexerState_Escape:
        {
            lexerState = LexerState::LexerState_String;
        }
        break;
        case LexerState_Delimiter:
        {
            isShouldAddCharacter = false;
            isTokenDone = true;
        }
        break;
        case LexerState_Comment:
        {
            isShouldAddCharacter = false;
            if (IsNewLine(character))
            {
                lexerState = LexerState::LexerState_Start;
            }
        }
        break;
        default:
        {
            printf("Undefined State\n");
            TokenError(lexerState, character, index, refLineCountForDebug);
            return LexerState::LexerState_Error;
        }


        }

        return lexerState;
    }

    XenonEnigne::XenonScriptAssemblerMachine::LexerState XenonScriptAssemblerMachine::TokenError(LexerState state, char character, unsigned int index, int lineCountForDebug) const
    {
        printf("Fetal Error: Lexer Error\n Character %c From State %d is undefined\n In the index %d and line %d", character, state, index, lineCountForDebug);
        return LexerState::LexerState_Error;
    }

    void XenonScriptAssemblerMachine::DetermineTokenType(Token* const token, LexerState currentState) const
    {
        switch (currentState)
        {
        default:
            break;
        case LexerState_Error:
            break;
        case LexerState_Done:
            break;
        case LexerState_Start:
            break;
        case LexerState_Identifier:
            token->m_tokenType = TokenType::TokenType_Identifier;
            for (int i = 0; i < m_instructionLookupList.Count(); i++)
            {
                if (token->m_character == m_instructionLookupList[i]->m_mnemonic)
                {
                    token->m_tokenType = TokenType::TokenType_Keyword;
                    token->m_keyword = m_instructionLookupList[i]->m_opType;
                    if (token->m_keyword == KeyWord::KeyWord_RETURNVALUE)
                    {
                        token->m_tokenType = TokenType::TokenType_Register;
                    }
                    break;
                }
            }
            break;
        case LexerState_String:
            token->m_tokenType = TokenType::TokenType_StringEntity;
            break;
        case LexerState_StringEnd:
            token->m_tokenType = TokenType::TokenType_StringEntity;
            break;
        case LexerState_Escape:
            break;
        case LexerState_Integral:
            token->m_tokenType = TokenType::TokenType_IntergalIiteral;
            break;
        case LexerState_Float:
            token->m_tokenType = TokenType::TokenType_FloatIiteral;
            break;
        case LexerState_Delimiter:
        {
            token->m_tokenType = TokenType::TokenType_Delimiter;
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
            case LexerState::LexerState_Done:
                return tokenVector;
            case LexerState::LexerState_Error:
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
            case TokenType_IntergalIiteral:
            {

            }
            break;
            case TokenType_FloatIiteral:
            {

            }
            break;
            case TokenType_Identifier:
            {
                // Line Label
                Token* token = PeekNextToken(*tokenVector, index);
                if (!token || token->m_tokenType != TokenType::TokenType_Delimiter)
                {
                    continue;
                }

                if (token->m_delimiter == DelimiterWord::DelimiterWord_Colon)
                {
                    Token* token = MoveToNextToken(*tokenVector, index);

                    LabelElement* const label = new LabelElement;
                    label->m_token = currentToken;
                    // Function return instruction will lead counter always bigger 1 than real instruction number.
                    label->m_instructionStreamIndex = instructionStreamCount - 1;
                    label->m_currentFunction = currentFunction;
                    m_labelTable.Add(label);
                }
            }
            break;
            case TokenType_StringEntity:
                break;
            case TokenType::TokenType_Label:
                break;
            case TokenType::TokenType_Function:
                break;
            case TokenType_Keyword:
            {
                switch (currentToken->m_keyword)
                {
                case KeyWord_INT:
                {
                    bool result = CreateSymbol(tokenVector, currentToken, InstructionOpType::InstructionOpType_IntegerLiteral, currentFunction, index, m_scriptHeader.m_globalDataSize);
                    if (!result)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }
                }
                break;
                case KeyWord_FLOAT:
                {
                    bool result = CreateSymbol(tokenVector, currentToken, InstructionOpType::InstructionOpType_FloatLiteral, currentFunction, index, m_scriptHeader.m_globalDataSize);
                    if (!result)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }
                }
                break;
                case KeyWord_FUNC:
                {
                    if (currentFunction)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    Token* token = MoveToNextToken(*tokenVector, index);
                    if (!token || token->m_tokenType != TokenType::TokenType_Identifier)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    token->m_tokenType = TokenType::TokenType_Function;

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
                    if (!token || token->m_tokenType != TokenType::TokenType_Delimiter)
                    {
                        if (!token || token->m_delimiter != DelimiterWord::DelimiterWord_OpenBrace)
                        {
                            BuildTableError(currentToken, index);
                            return false;
                        }
                    }

                    instructionStreamCount++;
                }
                break;
                case KeyWord_PARAM:
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
                    if (!token || token->m_tokenType != TokenType::TokenType_Identifier)
                    {
                        BuildTableError(currentToken, index);
                        return false;
                    }

                    currentFunction->m_parameterCount++;
                }
                break;
                case KeyWord_RETURNVALUE:
                    break;
                default:
                {
                    instructionStreamCount++;
                }
                }
            }
            break;
            case TokenType_Delimiter:
            {
                if (currentToken->m_delimiter == DelimiterWord::DelimiterWord_CloseBrace)
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
            case TokenType_HostAPI:
                break;
            case TokenType_Register:
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
        assert(true == false);
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
            case TokenType_IntergalIiteral:
            {
            }
            break;
            case TokenType_FloatIiteral:
                break;
            case TokenType_StringEntity:
                break;
            case TokenType_Label:
                break;
            case TokenType_Function:
            {
                //Token *token = MoveToNextToken(*tokenVector, index);
                FunctionElement*const functionElement = GetFunctionByName(currentToken->m_character);
                assert(functionElement != nullptr);
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
            case TokenType::TokenType_Keyword:
            {
                switch (currentToken->m_keyword)
                {
                case KeyWord::KeyWord_PARAM:
                {
                    currentToken = MoveToNextToken(*tokenVector, index);
                    assert(currentToken->m_tokenType == TokenType::TokenType_Keyword);

                    InstructionOpType variable = (currentToken->m_keyword == KeyWord::KeyWord_INT) ?
                        InstructionOpType::InstructionOpType_IntegerLiteral : InstructionOpType::InstructionOpType_FloatLiteral;

                    currentToken = MoveToNextToken(*tokenVector, index);
                    assert(currentToken->m_tokenType == TokenType::TokenType_Identifier);

                    currentFunctionParamCount++;
                    assert(currentFunctionParamCount <= currentFunction->m_parameterCount);
                    int stackIndex = -(Local_Stack_Start_Index + currentFunction->m_localDataSize + currentFunctionParamCount + Function_Return_Index_Space);

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
                case KeyWord_RETURNVALUE:
                {
                    assert(true == false);
                    continue; 
                }
                break;
                case KeyWord_INT:
                case KeyWord_FLOAT:
                case KeyWord_FUNC:
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

                for (int parameterIndex = 0; parameterIndex < currentInstrction->m_opCount;)
                {
                    currentToken = MoveToNextToken(*tokenVector, index);

                    switch (currentToken->m_tokenType)
                    {
                    case TokenType_IntergalIiteral:
                    case TokenType_FloatIiteral:
                    case TokenType_StringEntity:
                    case TokenType_Register:
                    case TokenType_Keyword:
                    case TokenType_Delimiter:
                    case TokenTypeCount:
                    {
                        if (currentInstructionLookup->m_opFlags[parameterIndex] & (1 << currentToken->m_tokenType) == 0)
                        {
                            CreateInstructionListError(currentToken, index);
                            return false;
                        }
                    }
                    }

                    switch (currentToken->m_tokenType)
                    {
                    case None:
                        break;
                    case TokenType_IntergalIiteral:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_IntegerLiteral;
                        currentInstrction->m_ops[parameterIndex]->m_integerLiteral = currentToken->m_character.ToInt();
                    }
                    break;
                    case TokenType_FloatIiteral:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_FloatLiteral;
                        currentInstrction->m_ops[parameterIndex]->m_floatLiteral = currentToken->m_character.ToFloat();
                    }
                    break;
                    case TokenType_StringEntity:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_StringIndex;
                        int indexOfString = m_stringTable.IndexOf(currentToken->m_character);
                        if (indexOfString == -1)
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
                    case TokenType::TokenType_Register:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_Register;
                        currentInstrction->m_ops[parameterIndex]->m_register = 0;
                    }
                    break;
                    case TokenType::TokenType_Identifier:
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
                                assert(currentToken->m_delimiter == DelimiterWord::DelimiterWord_OpenBracket);
                                currentToken = MoveToNextToken(*tokenVector, index);
                                assert(currentToken->m_tokenType == TokenType::TokenType_IntergalIiteral);

                                if (currentToken->m_tokenType == TokenType::TokenType_IntergalIiteral)
                                {
                                    currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_AbsoluteStackIndex;
                                    int arrayIndex = currentToken->m_character.ToInt();
                                    currentInstrction->m_ops[parameterIndex]->m_stackIndex = symbolElement->m_stackIndex + arrayIndex;
                                }
                                else if (currentToken->m_tokenType == TokenType::TokenType_Identifier)
                                {
                                    currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_RelativeStackIndex;
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
                                currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_AbsoluteStackIndex;
                                currentInstrction->m_ops[parameterIndex]->m_stackIndex = symbolElement->m_stackIndex;
                            }
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] &  1 << TokenType::TokenType_Function) > 0 &&
                            functionElement != nullptr)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_FunctionIndex;
                            currentInstrction->m_ops[parameterIndex]->m_stackIndex = functionElement->m_functionIndex;
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] & 1 << TokenType::TokenType_Label ) > 0 &&
                            labelElement != nullptr)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_InstructionIndex;
                            currentInstrction->m_ops[parameterIndex]->m_stackIndex = labelElement->m_instructionStreamIndex;
                        }
                        else if ((currentInstructionLookup->m_opFlags[parameterIndex] & 1 << TokenType::TokenType_HostAPI ) > 0)
                        {
                            currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_HostAPICallIndex;
                            int indexOfHostAPI = m_hostAPITable.IndexOf(currentToken->m_character);
                            if (indexOfHostAPI == -1)
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
                    case TokenType_Function:
                    {
                        currentInstrction->m_ops[parameterIndex]->m_type = InstructionOpType::InstructionOpType_FunctionIndex;
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
                    case TokenType_Delimiter:
                    {
                        if (parameterIndex < currentInstrction->m_opCount)
                        {
                            assert(currentToken->m_tokenType == TokenType_Delimiter && currentToken->m_delimiter == DelimiterWord_Comma);
                        }
                        else 
                        {
                            CreateInstructionListError(currentToken, index);
                            return false;
                        }
                    }
                        break;
                    case TokenTypeCount:
                    case TokenType_Label:
                    case TokenType_HostAPI:
                    default:
                    {
                        CreateInstructionListError(currentToken, index);
                        return false;
                    }
                    }

                    switch (currentToken->m_tokenType)
                    {
                    case TokenType_IntergalIiteral:
                    case TokenType_FloatIiteral:
                    case TokenType_StringEntity:
                    case TokenType_Register:
                    case TokenType_Identifier:
                    case TokenType_Function:
                    case TokenType_HostAPI:
                    {
                        parameterIndex++;
                    }
                    break;
                    default:
                    {
                        assert(currentToken->m_tokenType == TokenType_Delimiter);
                    }
                    }

                }
            }
            break;
            case TokenType_HostAPI:
                break;
            case TokenType_Register:
                break;
            case TokenType_Delimiter:
            {
                switch (currentToken->m_delimiter)
                {
                case DelimiterWord_Colon:
                    break;
                case DelimiterWord_SemiColon:
                    break;
                case DelimiterWord_OpenBracket:
                    break;
                case DelimiterWord_CloseBracket:
                    break;
                case DelimiterWord_Comma:
                    break;
                case DelimiterWord_OpenBrace:
                    break;
                case DelimiterWord_CloseBrace:
                {
                    currentInstrction = new Instruction;

                    if (currentFunction->m_functionIndex == m_scriptHeader.m_mainFunctionEntryIndex)
                    {
                        currentInstrction->m_opCode = KeyWord::KeyWord_EXIT;
                        currentInstrction->m_opCount = 1;
                        InstructionOp* const op = new InstructionOp();
                        op->m_type = InstructionOpType::InstructionOpType_IntegerLiteral;
                        op->m_integerLiteral = 0;
                        currentInstrction->m_ops.Add(op);
                    }
                    else
                    {
                        currentInstrction->m_opCode = KeyWord::KeyWord_RETURN;
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
        assert(true == false);
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
        char filePaht[200];
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
        Token* symbolToken = MoveToNextToken(*tokenVector, refIndex);
        if (!symbolToken || symbolToken->m_tokenType != TokenType::TokenType_Identifier)
        {
            BuildTableError(symbolToken, refIndex);
            return false;
        }

        int size = 1;
        Token* peekToken = PeekNextToken(*tokenVector, refIndex);
        if (!peekToken || peekToken->m_tokenType == TokenType::TokenType_Delimiter)
        {
            Token* token = MoveToNextToken(*tokenVector, refIndex);
            if (!token || token->m_delimiter != DelimiterWord::DelimiterWord_OpenBracket)
            {
                BuildTableError(token, refIndex);
                return false;
            }
            token = MoveToNextToken(*tokenVector, refIndex);
            if (!token || token->m_tokenType != TokenType::TokenType_IntergalIiteral)
            {
                BuildTableError(token, refIndex);
                return false;
            }
            size = token->m_character.ToInt();
            token = MoveToNextToken(*tokenVector, refIndex);
            if (!token || token->m_delimiter != DelimiterWord::DelimiterWord_OpenBracket)
            {
                BuildTableError(token, refIndex);
                return false;
            }
        }

        int stackIndex = 0;
        if (functionElement)
        {
            stackIndex = -(Local_Stack_Start_Index + functionElement->m_localDataSize);
        }
        else
        {
            stackIndex = refGlobalStackSize;
        }

        SymbolElement* symbol = new SymbolElement;
        symbol->m_variableType = tokenType;
        symbol->m_symbolToken = symbolToken;
        symbol->m_size = size;
        symbol->m_stackIndex = stackIndex;
        symbol->m_functionIndex = functionElement->m_functionIndex;

        m_symbolTable.Add(symbol);
        if (functionElement)
        {
            functionElement->m_localDataSize += size;
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

    XenonEnigne::Token* XenonScriptAssemblerMachine::MoveToNextToken(const TokenVector& tokenVector, int& index) const
    {
        assert(index + 1 < tokenVector.Count());
        return tokenVector[++index];
    }

    XenonEnigne::Token* XenonScriptAssemblerMachine::PeekNextToken(const TokenVector& tokenVector, int& index) const
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

    XenonEnigne::FunctionElement*const XenonScriptAssemblerMachine::GetFunctionByName(const String& functionName) const
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

    XenonEnigne::LabelElement*const XenonScriptAssemblerMachine::GetLabelByName(const String& labelName)
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
        {"int",  TokenType::TokenType_IntergalIiteral},
        {"float", TokenType::TokenType_FloatIiteral},
        {"string",  TokenType::TokenType_StringEntity},
        {"identifier",  TokenType::TokenType_Identifier},
        {"label",  TokenType::TokenType_Label},
        {"function",  TokenType::TokenType_Function},
        {"hostAPI",  TokenType::TokenType_HostAPI},
        {"register",   TokenType::TokenType_Register}
    };


}