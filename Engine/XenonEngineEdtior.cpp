#include "XenonEngineEdtior.h"
//#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/XenonKey.h"
#include "IO/InputSystem.h"
#include "Engine/VirtualMachine/XenonCompiler.h"

namespace XenonEngine
{
	using namespace Algorithm;


	void EditorModeState::Enter(FiniteStateMachine& /*fsm*/)
	{
	}

	void EditorModeState::Update(FiniteStateMachine& fsm)
	{
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_P))
		{
			fsm.NextState(EditorStatus::RuntimeMode);
		}
	}

	void EditorModeState::Exit(FiniteStateMachine& /*fsm*/)
	{
	}

	void RuntimeModeState::Enter(FiniteStateMachine& /*fsm*/)
	{
		compiler = new XenonCompiler;
		compiler->Initialize();
	}

	void RuntimeModeState::Update(FiniteStateMachine& fsm)
	{
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_P))
		{
			fsm.NextState(EditorStatus::EditorMode);
		}

		compiler->RunScript();
	}

	void RuntimeModeState::Exit(FiniteStateMachine& /*fsm*/)
	{
		delete compiler;
	}


	void XenonEngineEditor::Initialize()
	{
		FSMState* edtiorState = new FSMState(EditorStatus::EditorMode, "Editor Mode");
		FSMState* runtimeState = new FSMState(EditorStatus::RuntimeMode, "Runtime Mode");
		m_editorStateMachine.RegisterState(edtiorState);
		m_editorStateMachine.RegisterState(runtimeState);

		Vector<int> editorStates;
		editorStates.Add(EditorStatus::EditorMode);
		editorStates.Add(EditorStatus::RuntimeMode);
		m_editorStateMachine.SetStates(editorStates);

		m_editorStateMachine.EnterState(EditorStatus::EditorMode);

		//Vector<String> editorStateRelatedState;
		//editorStateRelatedState.Add("Runtime Mode");
		//m_editorStateMachine.RegisterRelation("Editor Mode", editorStateRelatedState);

		//Vector<String> runtimeStateRelatedState;
		//runtimeStateRelatedState.Add("Editor Mode");
		//m_editorStateMachine.RegisterRelation("Runtime Mode", runtimeStateRelatedState);
	}

	void XenonEngineEditor::Update()
	{
		m_editorStateMachine.Update();
	}

	void XenonEngineEditor::Shutdown()
	{
	}

	bool XenonEngineEditor::IsEditorMode() const
	{
		return m_editorStateMachine.FocusStateEnum() == EditorStatus::EditorMode;
	}

}