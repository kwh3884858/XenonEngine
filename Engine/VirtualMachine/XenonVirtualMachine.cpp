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