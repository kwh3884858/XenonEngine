#include "Engine/VirtualMachine/XenonVirtualMachine.h"

#include "Engine/FileManager/FileManager.h"
#include "Engine/GameObjectWorldManager.h"
#include "Engine/GameObjectWorld.h"
#include "Engine/GameObject.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Rigidbody2D.h"

#include "CrossPlatform/XenonFile.h"

#include "Algorithms/Vector.h"
#include "Algorithms/StreamingVector.h"

#include <cstdio>
namespace XenonEngine
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
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex]->m_type, sizeof(m_instructionList[index]->m_ops[opIndex]->m_type));
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex]->m_integerLiteral, sizeof(m_instructionList[index]->m_ops[opIndex]->m_integerLiteral));
				streamIndex = streamedFile->Read(streamIndex, &m_instructionList[index]->m_ops[opIndex]->m_offsetIndex, sizeof(m_instructionList[index]->m_ops[opIndex]->m_offsetIndex));
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
				streamIndex = streamedFile->Read(streamIndex, &m_functionTable[index]->m_localDataSize, sizeof(m_functionTable[index]->m_localDataSize));
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
				streamIndex = streamedFile->Read(streamIndex, tmpString, stringLength);
				m_stringTable.Add(String(tmpString, stringLength));
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
				streamIndex = streamedFile->Read(streamIndex, tmpString, stringLength);
				m_hostAPITable.Add(String(tmpString, stringLength));
				delete[] tmpString;
			}
		}
	}

    bool XenonVirtualMachine::RunScript()
{
        assert(m_scriptHeader.m_mainFunctionEntryIndex >= 0);

        Value* globalData = nullptr;
        if (m_scriptHeader.m_globalDataSize > 0)
        {
            globalData = new Value[m_scriptHeader.m_globalDataSize];
        }

        for (int index = 0; index < m_instructionList.Count(); )
        {
			int currentIndex = index;
            Instruction* instruction = m_instructionList[index];
            switch (instruction->m_opCode)
            {
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
                InstructionOp& op0 = const_cast<InstructionOp&>(ResolveInstructionOp(currentIndex, 0));
				const InstructionOp& op1 = ResolveInstructionOp(currentIndex, 1);

				switch (instruction->m_opCode)
				{
				case KeyWord_MOV:
				{
					if (&op0 == &op1)
					{
						break;
					}
					op0 = op1;
				}
				break;
				case KeyWord_ADD:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type==InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral += op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						op0.m_floatLiteral += op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SUB:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral -= op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						op0.m_floatLiteral -= op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_MUL:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral *= op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						op0.m_floatLiteral *= op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_DIV:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral /= op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						op0.m_floatLiteral /= op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_MOD:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral %= op1.m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_EXP:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral = (int)pow (op0.m_integerLiteral, op1.m_integerLiteral);
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						op0.m_floatLiteral = (float)pow(op0.m_integerLiteral, op1.m_integerLiteral);
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_AND:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral &= op1.m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_OR:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral |= op1.m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_XOR:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral ^= op1.m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SHL:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral <<= op1.m_integerLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_SHR:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						op0.m_integerLiteral >>= op1.m_integerLiteral;
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
				InstructionOp& op = const_cast<InstructionOp&>( ResolveInstructionOp(currentIndex, 0));
				switch (instruction->m_opCode)
				{
				case KeyWord_NEG:
				{
					if (op.m_type == InstructionOpType_IntegerLiteral)
					{
						op.m_integerLiteral = -op.m_integerLiteral;
					}
					else if (op.m_type == InstructionOpType_FloatLiteral)
					{
						op.m_floatLiteral = -op.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_INC:
				{
					if (op.m_type == InstructionOpType_IntegerLiteral)
					{
						++op.m_integerLiteral ;
					}
					else if (op.m_type == InstructionOpType_FloatLiteral)
					{
						++op.m_floatLiteral ;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_DEC: {
					if (op.m_type == InstructionOpType_IntegerLiteral)
					{
						--op.m_integerLiteral ;
					}
					else if (op.m_type == InstructionOpType_FloatLiteral)
					{
						--op.m_floatLiteral ;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_NOT: {
					if (op.m_type == InstructionOpType_IntegerLiteral)
					{
						op.m_integerLiteral = ~op.m_integerLiteral;
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
				InstructionOp& op0 = const_cast<InstructionOp&>(ResolveInstructionOp(currentIndex, 0));
				const InstructionOp& op1 = ResolveInstructionOp(currentIndex, 1);

				if (op0.m_type == InstructionOpType_StringIndex && op1.m_type == InstructionOpType_StringIndex)
				{
					String newString = m_stringTable[op0.m_stackIndex];
					newString.Append(m_stringTable[op1.m_stackIndex]);
					op0.m_stringTableIndex = m_stringTable.Count();
					m_stringTable.Add(newString);
				}
			}
                break;
            case KeyWord_GETCHAR:
			{
				InstructionOp& op0 = const_cast<InstructionOp&>(ResolveInstructionOp(currentIndex, 0));
				const InstructionOp& op1 = ResolveInstructionOp(currentIndex, 1);
				const InstructionOp& op2 = ResolveInstructionOp(currentIndex, 2);
				if (op0.m_type == InstructionOpType_StringIndex 
					&& op1.m_type == InstructionOpType_StringIndex
					&& op2.m_type == InstructionOpType_IntegerLiteral)
				{
					String searchedString = m_stringTable[op1.m_stackIndex];
					char character = searchedString[op2.m_integerLiteral];
					String newStirng(character);
					op0.m_stringTableIndex = m_stringTable.Count();
					m_stringTable.Add(newStirng);
				}
			}
                break;
            case KeyWord_SETCHAR:
			{
				InstructionOp& op0 = const_cast<InstructionOp&>(ResolveInstructionOp(currentIndex, 0));
				const InstructionOp& op1 = ResolveInstructionOp(currentIndex, 1);
				const InstructionOp& op2 = ResolveInstructionOp(currentIndex, 2);
				if (op0.m_type == InstructionOpType_StringIndex
					&& op1.m_type == InstructionOpType_IntegerLiteral
					&& op2.m_type == InstructionOpType_StringIndex)
				{
					char character = m_stringTable[op2.m_stackIndex][0];
					m_stringTable[op0.m_stackIndex][op1.m_integerLiteral] = character;
				}
			}
                break;
            case KeyWord_JMP:
			{
				const InstructionOp& op = ResolveInstructionOp(currentIndex, 0);
				if (op.m_type == InstructionOpType_InstructionIndex)
				{
					currentIndex = op.m_instructionIndex;
				}
			}
                break;
            case KeyWord_JE:
            case KeyWord_JNE:
            case KeyWord_JG:
            case KeyWord_JL:
            case KeyWord_JGE:
            case KeyWord_JLE:
			{
				const InstructionOp& op0 = ResolveInstructionOp(currentIndex, 0);
				const InstructionOp& op1 = ResolveInstructionOp(currentIndex, 1);
				const InstructionOp& op2 = ResolveInstructionOp(currentIndex, 2);

				bool isShouldJump = false;
				switch (instruction->m_opCode)
				{
				case KeyWord_JE:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral == op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral == op1.m_floatLiteral;
					}
					else if (op0.m_type == InstructionOpType_StringIndex && op1.m_type == InstructionOpType_StringIndex)
					{
						if (op0.m_stringTableIndex == op1.m_stringTableIndex)
						{
							isShouldJump = true;
						}
						String lhs = ResolveOpAsString(currentIndex, 0);
						String rhs = ResolveOpAsString(currentIndex, 1);
						if (lhs == rhs)
						{
							isShouldJump = true;
						}
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_JNE:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral != op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral != op1.m_floatLiteral;
					}
					else if (op0.m_type == InstructionOpType_StringIndex && op1.m_type == InstructionOpType_StringIndex)
					{
						if (op0.m_stringTableIndex != op1.m_stringTableIndex)
						{
							isShouldJump = true;
						}
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_JG:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral > op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral > op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_JL:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral < op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral < op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_JGE:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral >= op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral >= op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				case KeyWord_JLE:
				{
					if (op0.m_type == InstructionOpType_IntegerLiteral && op1.m_type == InstructionOpType_IntegerLiteral)
					{
						isShouldJump = op0.m_integerLiteral <= op1.m_integerLiteral;
					}
					else if (op0.m_type == InstructionOpType_FloatLiteral && op1.m_type == InstructionOpType_FloatLiteral)
					{
						isShouldJump = op0.m_floatLiteral <= op1.m_floatLiteral;
					}
					else
					{
						assert(true == false);
					}
				}
				break;
				}
				if (isShouldJump)
				{
					if (op2.m_type == InstructionOpType_InstructionIndex)
					{
						currentIndex = op2.m_instructionIndex;
					}
					else
					{
						assert(true == false);
					}
				}
			}
                break;
            case KeyWord_PUSH:
			{
				const InstructionOp& op = ResolveInstructionOp(currentIndex, 0);
				m_localStack.Push(op);
			}
                break;
            case KeyWord_POP:
			{
				InstructionOp& op = const_cast<InstructionOp&>(ResolveInstructionOp(currentIndex, 0));
				op = m_localStack.Pop();
			}
                break;
            case KeyWord_CALL:
			{
				InstructionOp returnAddress;
				returnAddress.m_type = InstructionOpType_InstructionIndex;
				returnAddress.m_instructionIndex = currentIndex +1;
				m_localStack.Push(returnAddress);

				const InstructionOp& functionOp = ResolveInstructionOp(currentIndex, 0);
				FunctionElement* function = m_functionTable[functionOp.m_stackIndex];
				PushFrame(function->m_localDataSize);
				m_localStack.Push(functionOp);

				currentIndex = function->m_entryPoint;

                m_currentFrameTopIndex = m_localStack.Count();
			}
                break;
            case KeyWord_CALLHOST:
			{
				const InstructionOp& hostAPI = ResolveInstructionOp(currentIndex, 0);
				if (m_hostAPITable[hostAPI.m_hostAPICallIndex] == "DebugPrint")
				{
					InstructionOp op1 = m_localStack.Pop();
					InstructionOp op2 = m_localStack.Pop();
					if (op1.m_type == InstructionOpType_StringIndex && op2.m_type == InstructionOpType_IntegerLiteral)
					{
						char tmp[200];
						m_stringTable[op1.m_stringTableIndex].CString(tmp);
						printf("%s\n", tmp);
					}
					else
					{
						assert(true == false);
					}
				}
                else if (m_hostAPITable[hostAPI.m_hostAPICallIndex] == "SetVelocity")
				{
                    InstructionOp op1 = m_localStack.Pop();
                    if (op1.m_type == InstructionOpType_FloatLiteral)
                    {
                        XenonEngine::GameObjectWorld* world = GameObjectWorldManager::Get().GetCurrentWorld();
                        XenonEngine::GameObject* player = world->GetGameObject("Player");
                        XenonEngine::PlayerPersonality* personlity = player->GetComponent<XenonEngine::PlayerPersonality>();
                        personlity->SetVelocity(op1.m_floatLiteral);
                    }
                    else
                    {
                        assert(true == false);
                    }
				}
                else if (m_hostAPITable[hostAPI.m_hostAPICallIndex] == "SetJumpForce")
                {
                    InstructionOp op1 = m_localStack.Pop();
                    if (op1.m_type == InstructionOpType_FloatLiteral)
                    {
                        XenonEngine::GameObjectWorld* world = GameObjectWorldManager::Get().GetCurrentWorld();
                        XenonEngine::GameObject* player = world->GetGameObject("Player");
                        XenonEngine::PlayerPersonality* personlity = player->GetComponent<XenonEngine::PlayerPersonality>();
                        personlity->SetJumpForce(op1.m_floatLiteral);
                    }
                    else
                    {
                        assert(true == false);
                    }
                }
                else if (m_hostAPITable[hostAPI.m_hostAPICallIndex] == "SetMass")
                {
                    InstructionOp op1 = m_localStack.Pop();
                    if (op1.m_type == InstructionOpType_FloatLiteral)
                    {
                        XenonEngine::GameObjectWorld* world = GameObjectWorldManager::Get().GetCurrentWorld();
                        XenonEngine::GameObject* player = world->GetGameObject("Player");
                        XenonEngine::Rigidbody2D* rigid = player->GetComponent<XenonEngine::Rigidbody2D>();
                        rigid->SetMass(op1.m_floatLiteral);
                    }
                    else
                    {
                        assert(true == false);
                    }
                }
				else if (m_hostAPITable[hostAPI.m_hostAPICallIndex] == "SetBulletForce")
				{
					InstructionOp op1 = m_localStack.Pop();
					if (op1.m_type == InstructionOpType_FloatLiteral)
					{
						XenonEngine::GameObjectWorld* world = GameObjectWorldManager::Get().GetCurrentWorld();
						XenonEngine::GameObject* player = world->GetGameObject("Player");
						XenonEngine::PlayerPersonality* personlity = player->GetComponent<XenonEngine::PlayerPersonality>();
						personlity->SetBulletForce(op1.m_floatLiteral);
					}
					else
					{
						assert(true == false);
					}
				}
			}
                break;
            case KeyWord_PAUSE:
                break;
            case KeyWord_RETURN:
            {
                InstructionOp functionOp = m_localStack.Pop();
                assert(functionOp.m_type == InstructionOpType_FunctionIndex);
                assert(functionOp.m_funcIndex >= 0);
                FunctionElement* function = m_functionTable[functionOp.m_stackIndex];
                PopFrame(function->m_localDataSize);

                InstructionOp returnAddress = m_localStack.Pop();
                assert(returnAddress.m_type == InstructionOpType_InstructionIndex);
                currentIndex = returnAddress.m_instructionIndex;

                PopFrame(function->m_parameterCount);

                m_currentFrameTopIndex = m_localStack.Count();
            }
                break;
            case KeyWord_EXIT:
            {
                const InstructionOp& exitValue = ResolveInstructionOp(currentIndex, 0);
                printf("File Exit: [ %d ]\n", exitValue.m_integerLiteral);
                return true;
            }
                break;
            case KeyWord_INT:
            case KeyWord_FLOAT:
            case KeyWord_FUNC:
            case KeyWord_PARAM:
            case KeyWord_RETURNVALUE:
            default:
            {
                assert(true == false);
            }
                break;
            }
			if (currentIndex == index)
			{
				index++;
			}
			else
			{
				index = currentIndex;
			}
        }

        return false;
    }

	const InstructionOp& XenonVirtualMachine::GetInstructionByStackIndex(int index)const
	{
		int realIndex = index < 0 ? m_currentFrameTopIndex + index : index;
		if (index <0)
		{
            
            return m_localStack[realIndex];
		}
		else
		{
            return m_glabalStack[realIndex];
		}
	}

    const InstructionOp& XenonVirtualMachine::ResolveInstructionOp(int instructionIndex, int opIndex) const
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
			const InstructionOp& relativeOp = GetInstructionByStackIndex(op->m_offsetIndex);
			assert(relativeOp.m_type == InstructionOpType_IntegerLiteral);
			return GetInstructionByStackIndex(op->m_stackIndex + relativeOp.m_integerLiteral);
		}
			break;
        case InstructionOpType_Register:
        {
            return m_returnValue;
        }
            break;
		}
		return *op;
	}

	void XenonVirtualMachine::PushFrame(int size)
	{
        for (int i = 0; i < size; i++)
        {
            m_localStack.Push(InstructionOp());
        }
	}

	void XenonVirtualMachine::PopFrame(int size)
	{
		for (int i = 0; i < size; i++)
		{
			m_localStack.Pop();
		}
	}

	int XenonVirtualMachine::ResolveOpAsInteger(int instructionIndex, int opIndex) const
	{
		assert(instructionIndex < m_instructionList.Count());
		assert(opIndex < m_instructionList[instructionIndex]->m_ops.Count());

		switch (m_instructionList[instructionIndex]->m_ops[opIndex]->m_type)
		{
		case XenonEngine::InstructionOpType_IntegerLiteral:
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
		case XenonEngine::InstructionOpType_FloatLiteral:
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
		case XenonEngine::InstructionOpType_StringIndex:
		{
			int stringIndex =m_instructionList[instructionIndex]->m_ops[opIndex]->m_stringTableIndex;
			assert(stringIndex < m_stringTable.Count());
			return m_stringTable[stringIndex];
		}
		default:
		{
			assert(true == false);
			return String("Error");
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