#include "Engine/VirtualMachine/XenonVirtualMachine.h"

#include "Engine/FileManager/FileManager.h"
#include "CrossPlatform/XenonFile.h"

#include "Algorithms/Vector.h"
#include "Algorithms/StreamingVector.h"
namespace XenonEnigne
{
	using Algorithm::Vector;
	using CrossPlatform::XenonFile;
	using Algorithm::StreamingVector;

	bool XenonVirtualMachine::Initialize()
	{
		XenonFile* const fileContent = FileManager::Get().ReadFile("main.xon");

		return true;
	}

	bool XenonVirtualMachine::LoadScript(Algorithm::StreamingVector<char>* streamedFile)
	{
		int streamIndex = 0;
		streamIndex = streamedFile->Read(streamIndex, &m_scriptHeader.m_globalDataSize, sizeof(m_scriptHeader.m_globalDataSize));
		streamIndex = streamedFile->Read(streamIndex, &m_scriptHeader.m_mainFunctionEntryIndex, sizeof(m_scriptHeader.m_mainFunctionEntryIndex));

		if (m_scriptHeader.m_mainFunctionEntryIndex < 0)
		{
			assert(true == false);
			return false;
		}

		int instructionCount = 0;
		streamIndex = streamedFile->Read(streamIndex, &instructionCount, sizeof(instructionCount));
		for (int index = 0; index < instructionCount; index++)
		{
			m_instructionList.Add(new Instruction);
			streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_opCode, sizeof(m_instructionList[index]->m_opCode));
			streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_opCount, sizeof(m_instructionList[index]->m_opCount));
			for (int opIndex = 0; opIndex < m_instructionList[index]->m_opCount; opIndex++)
			{
				m_instructionList[index]->m_ops.Add(new InstructionOp);
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex], sizeof(m_instructionList[index]->m_ops[opIndex]));
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex]->m_interalLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_interalLiteral));
			}
		}

		{
			int count = 0;
			streamIndex = streamedFile->Read(streamIndex, &count, sizeof(count));
			for (int index = 0; index < count; index++)
			{
				m_functionTable.Add(new FunctionElement);
				streamIndex = streamedFile->Read(streamIndex, &m_functionTable[index]->m_functionIndex, sizeof(m_functionTable[index]->m_functionIndex));
				streamIndex = streamedFile->Read(streamIndex, &m_functionTable[index]->m_entryPoint, sizeof(m_functionTable[index]->m_entryPoint));
				streamIndex = streamedFile->Read(streamIndex, &m_functionTable[index]->m_localStackSize, sizeof(m_functionTable[index]->m_localStackSize));
				streamIndex = streamedFile->Read(streamIndex, &m_functionTable[index]->m_parameterCount, sizeof(m_functionTable[index]->m_parameterCount));
			}
		}

		{
			int count = 0;
			streamIndex = streamedFile->Read(streamIndex, &count, sizeof(count));
			for (int index = 0; index < count; index++)
			{
				m_labelTable.Add(new LabelElement);
				streamIndex = streamedFile->Read(streamIndex, &m_labelTable[index]->m_instructionStreamIndex, sizeof(m_labelTable[index]->m_instructionStreamIndex));
			}
		}

		{
			int count = 0;
			streamIndex = streamedFile->Read(streamIndex, &count, sizeof(count));
			for (int index = 0; index < count; index++)
			{
				int stringLength = 0;
				streamIndex = streamedFile->Read(streamIndex, &stringLength, sizeof(stringLength));
				assert(stringLength != 0);
				char* tmpString = new char[stringLength];
				streamIndex = streamedFile->Read(streamIndex, &tmpString, sizeof(tmpString));
				m_stringTable.Add(String(tmpString));
				delete[] tmpString;
			}
		}

		{
			int count = 0;
			streamIndex = streamedFile->Read(streamIndex, &count, sizeof(count));
			for (int index = 0; index < count; index++)
			{
				int stringLength = 0;
				streamIndex = streamedFile->Read(streamIndex, &stringLength, sizeof(stringLength));
				assert(stringLength != 0);
				char* tmpString = new char[stringLength];
				streamIndex = streamedFile->Read(streamIndex, &tmpString, sizeof(tmpString));
				m_hostAPITable.Add(String(tmpString));
				delete[] tmpString;
			}
		}
	}

    void XenonVirtualMachine::RunScript()
    {
        assert(m_scriptHeader.m_mainFunctionEntryIndex < 0);

        Value* globalData = nullptr;
        if (m_scriptHeader.m_globalDataSize > 0)
        {
            globalData = new Value[m_scriptHeader.m_globalDataSize];
        }

        for (int i = 0; i < m_instructionList.Count(); i++)
        {
            Instruction* instruction = m_instructionList[i];
            switch (instruction->m_opCode)
            {
            default:
                break;
            case KeyWord_INT:
            {
                for (int opIndex = 0;opIndex < instruction->m_opCount; opIndex++)
                {

                }
            }
                break;
            case KeyWord_FLOAT:
                break;
            case KeyWord_MOV:
                break;
            case KeyWord_ADD:
                break;
            case KeyWord_SUB:
                break;
            case KeyWord_MUL:
                break;
            case KeyWord_DIV:
                break;
            case KeyWord_MOD:
                break;
            case KeyWord_EXP:
                break;
            case KeyWord_NEG:
                break;
            case KeyWord_INC:
                break;
            case KeyWord_DEC:
                break;
            case KeyWord_AND:
                break;
            case KeyWord_OR:
                break;
            case KeyWord_XOR:
                break;
            case KeyWord_NOT:
                break;
            case KeyWord_SHL:
                break;
            case KeyWord_SHR:
                break;
            case KeyWord_CONCAT:
                break;
            case KeyWord_GETCHAR:
                break;
            case KeyWord_SETCHAR:
                break;
            case KeyWord_JMP:
                break;
            case KeyWord_JE:
                break;
            case KeyWord_JNE:
                break;
            case KeyWord_JG:
                break;
            case KeyWord_JL:
                break;
            case KeyWord_JGE:
                break;
            case KeyWord_JLE:
                break;
            case KeyWord_PUSH:
                break;
            case KeyWord_POP:
                break;
            case KeyWord_FUNC:
                break;
            case KeyWord_PARAM:
                break;
            case KeyWord_CALL:
                break;
            case KeyWord_RET:
                break;
            case KeyWord_CALLHOST:
                break;
            case KeyWord_PAUSE:
                break;
            case KeyWord_EXIT:
                break;
            }
        }
    }

    //bool XenonVirtualMachine::Laxer(const char* const content, Token** const tokens)
	//{
	//    unsigned int index = 0;
	//    char currentChar = content[index];
	//    Vector< Token* > outputTokens;
	//    while (currentChar != '\0') {
	//        switch (currentChar)
	//        {
	//        case '(':
	//            
	//        default:
	//            break;
	//        }
	//    }
	//    return true;
	//}

}