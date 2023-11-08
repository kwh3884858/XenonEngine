#include "XenonEngineEdtior.h"
//#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/XenonKey.h"
#include "IO/InputSystem.h"

namespace XenonEngine
{
	using namespace Algorithm;


	void EditorModeState::Enter(FiniteStateMachine& fsm) const
	{
	}

	void EditorModeState::Update(FiniteStateMachine& fsm) const
	{
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_P))
		{
			fsm.NextState(EditorStatus::RuntimeMode);
		}
	}

	void EditorModeState::Exit(FiniteStateMachine& fsm) const
	{
	}

	void RuntimeModeState::Enter(FiniteStateMachine& fsm) const
	{
	}

	void RuntimeModeState::Update(FiniteStateMachine& fsm) const
	{
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_P))
		{
			fsm.NextState(EditorStatus::EditorMode);
		}
	}

	void RuntimeModeState::Exit(FiniteStateMachine& fsm) const
	{
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