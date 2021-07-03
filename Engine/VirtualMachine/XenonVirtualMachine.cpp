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
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex]->m_integerLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_integerLiteral));
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

        for (int index = 0; index < m_instructionList.Count(); index++)
        {
            Instruction* instruction = m_instructionList[index];
            switch (instruction->m_opCode)
            {
            default:
                break;
            case KeyWord_INT:
            {
                //InstructionOpType opType = m_instructionList[index]->m_ops[0]->m_type;
                //InstructionOpType opType = m_instructionList[index]->m_ops[1]->m_type;
            }
                break;
            case KeyWord_FLOAT:
                break;
            case KeyWord_MOV:
            case KeyWord_ADD:
            case KeyWord_SUB:
            case KeyWord_MUL:
            case KeyWord_DIV:
            case KeyWord_MOD:
            case KeyWord_EXP:
			case KeyWord_AND:
			case KeyWord_OR:
			case KeyWord_XOR:
			case KeyWord_SHL:
			case KeyWord_SHR:
			{
				InstructionOp* op0 = ResolveInstructionOp(index, 0);
				InstructionOp* op1 = ResolveInstructionOp(index, 1);

				switch (instruction->m_opCode)
				{
				case KeyWord_MOV:
				{
					if (op0 == op1)
					{
						break;
					}
					*op0 = *op1;
				}
				break;
				case KeyWord_ADD:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type==InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral += op1->m_integerLiteral;
					}
					else if (op0->m_type == InstructionOpType_FloatLiteral && op1->m_type == InstructionOpType_FloatLiteral)
					{
						op0->m_floatLiteral += op1->m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SUB:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral -= op1->m_integerLiteral;
					}
					else if (op0->m_type == InstructionOpType_FloatLiteral && op1->m_type == InstructionOpType_FloatLiteral)
					{
						op0->m_floatLiteral -= op1->m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_MUL:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral *= op1->m_integerLiteral;
					}
					else if (op0->m_type == InstructionOpType_FloatLiteral && op1->m_type == InstructionOpType_FloatLiteral)
					{
						op0->m_floatLiteral *= op1->m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_DIV:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral /= op1->m_integerLiteral;
					}
					else if (op0->m_type == InstructionOpType_FloatLiteral && op1->m_type == InstructionOpType_FloatLiteral)
					{
						op0->m_floatLiteral /= op1->m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_MOD:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral %= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_EXP:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral = (int)pow (op0->m_integerLiteral, op1->m_integerLiteral);
					}
					else if (op0->m_type == InstructionOpType_FloatLiteral && op1->m_type == InstructionOpType_FloatLiteral)
					{
						op0->m_floatLiteral = (float)pow(op0->m_integerLiteral, op1->m_integerLiteral);
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_AND:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral &= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_OR:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral |= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_XOR:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral ^= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SHL:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral <<= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SHR:
				{
					if (op0->m_type == InstructionOpType_IntegerLiteral && op1->m_type == InstructionOpType_IntegerLiteral)
					{
						op0->m_integerLiteral >>= op1->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;

				}
			}
                break;
            case KeyWord_NEG:
            case KeyWord_INC:
            case KeyWord_DEC:
            case KeyWord_NOT:
			{
				InstructionOp* op = ResolveInstructionOp(index, 0);
				switch (instruction->m_opCode)
				{
				case KeyWord_NEG:
				{
					if (op->m_type == InstructionOpType_IntegerLiteral)
					{
						op->m_integerLiteral = -op->m_integerLiteral;
					}
					else if (op->m_type == InstructionOpType_FloatLiteral)
					{
						op->m_floatLiteral = -op->m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_INC:
				{
					if (op->m_type == InstructionOpType_IntegerLiteral)
					{
						++op->m_integerLiteral ;
					}
					else if (op->m_type == InstructionOpType_FloatLiteral)
					{
						++op->m_floatLiteral ;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_DEC: {
					if (op->m_type == InstructionOpType_IntegerLiteral)
					{
						--op->m_integerLiteral ;
					}
					else if (op->m_type == InstructionOpType_FloatLiteral)
					{
						--op->m_floatLiteral ;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_NOT: {
					if (op->m_type == InstructionOpType_IntegerLiteral)
					{
						op->m_integerLiteral = ~op->m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				default:
					break;
				}
			}
                break;

            case KeyWord_CONCAT:
			{
				InstructionOp* op0 = ResolveInstructionOp(index, 0);
				InstructionOp* op1 = ResolveInstructionOp(index, 1);

				if (op0->m_type == InstructionOpType_StringIndex && op1->m_type == InstructionOpType_StringIndex)
				{
					String newString = m_stringTable[op0->m_stackIndex];
					newString.Append(m_stringTable[op1->m_stackIndex]);
					op0->m_stringTableIndex = m_stringTable.Count();
					m_stringTable.Add(newString);
				}
			}
                break;
            case KeyWord_GETCHAR:
			{
				InstructionOp* op0 = ResolveInstructionOp(index, 0);
				InstructionOp* op1 = ResolveInstructionOp(index, 1);
				InstructionOp* op2 = ResolveInstructionOp(index, 2);
				if (op0->m_type == InstructionOpType_StringIndex 
					&& op1->m_type == InstructionOpType_StringIndex
					&& op2->m_type == InstructionOpType_IntegerLiteral)
				{
					String searchedString = m_stringTable[op1->m_stackIndex];
					char character = searchedString[op2->m_integerLiteral];
					String newStirng(character);
					op0->m_stringTableIndex = m_stringTable.Count();
					m_stringTable.Add(newStirng);
				}
			}
                break;
            case KeyWord_SETCHAR:
			{
				InstructionOp* op0 = ResolveInstructionOp(index, 0);
				InstructionOp* op1 = ResolveInstructionOp(index, 1);
				InstructionOp* op2 = ResolveInstructionOp(index, 2);
				if (op0->m_type == InstructionOpType_StringIndex)
				{

				}
			}
                break;
            case KeyWord_JMP:
                break;
            case KeyWord_JE:
            case KeyWord_JNE:
            case KeyWord_JG:
            case KeyWord_JL:
            case KeyWord_JGE:
            case KeyWord_JLE:
			{

			}
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

	InstructionOp* XenonVirtualMachine::GetInstructionByStackIndex(int index)const
	{
		int realIndex = index < 0 ? -index + 1 : index;
		if (index <0)
		{
			m_localStack[realIndex];
		}
		else
		{
			m_glabalStack[realIndex];
		}
	}

	InstructionOp* XenonVirtualMachine::ResolveInstructionOp(int instructionIndex, int opIndex) const
	{
		assert(instructionIndex < m_instructionList.Count());
		assert(opIndex < m_instructionList[instructionIndex]->m_ops.Count());

		InstructionOp* op = m_instructionList[instructionIndex]->m_ops[opIndex];
		switch (op->m_type)
		{
		case InstructionOpType_AbsoluteStackIndex:
		{
			return GetInstructionByStackIndex(op->m_stackIndex);
		}
			break;
		case InstructionOpType_RelativeStackIndex:
		{
			InstructionOp* relativeOp = GetInstructionByStackIndex(op->m_offsetIndex);
			assert(relativeOp->m_type == InstructionOpType_IntegerLiteral);
			return GetInstructionByStackIndex(op->m_stackIndex + relativeOp->m_integerLiteral);
		}
			break;
		}
		return nullptr;
	}

	int XenonVirtualMachine::ResolveOpAsInteger(int instructionIndex, int opIndex) const
	{
		assert(instructionIndex < m_instructionList.Count());
		assert(opIndex < m_instructionList[instructionIndex]->m_ops.Count());

		switch (m_instructionList[instructionIndex]->m_ops[opIndex]->m_type)
		{
		case XenonEnigne::InstructionOpType_IntegerLiteral:
		{
			return m_instructionList[instructionIndex]->m_ops[opIndex]->m_integerLiteral;
		}
		default:
		{
			assert(true == false);
			return 0;
		}
		}
	}

	float XenonVirtualMachine::ResolveOpAsFloat(int instructionIndex, int opIndex) const
	{
		assert(instructionIndex < m_instructionList.Count());
		assert(opIndex < m_instructionList[instructionIndex]->m_ops.Count());

		switch (m_instructionList[instructionIndex]->m_ops[opIndex]->m_type)
		{
		case XenonEnigne::InstructionOpType_FloatLiteral:
		{
			return m_instructionList[instructionIndex]->m_ops[opIndex]->m_floatLiteral;
		}
		default:
		{
			assert(true == false);
			return 0;
		}
		}
	}

	String XenonVirtualMachine::ResolveOpAsString(int instructionIndex, int opIndex) const
	{
		assert(instructionIndex < m_instructionList.Count());
		assert(opIndex < m_instructionList[instructionIndex]->m_ops.Count());

		switch (m_instructionList[instructionIndex]->m_ops[opIndex]->m_type)
		{
		case XenonEnigne::InstructionOpType_FloatLiteral:
		{
			int stringIndex =m_instructionList[instructionIndex]->m_ops[opIndex]->m_stringTableIndex;
			assert(stringIndex < m_stringTable.Count());
			return m_stringTable[stringIndex];
		}
		default:
		{
			assert(true == false);
			return 0;
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