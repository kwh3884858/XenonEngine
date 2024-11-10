#pragma once

#include "Algorithms/FiniteStateMachine.h"
#include "VirtualMachine/XenonCompiler.h"

using Algorithm::FSMState;
using Algorithm::FiniteStateMachine;

namespace XenonEngine
{
	enum EditorStatus
	{
		EditorMode,
		RuntimeMode,
	};

	class EditorModeState : FSMState
	{
	public:
		virtual void Enter(FiniteStateMachine& fsm)  override;
		virtual void Update(FiniteStateMachine& fsm) override;
		virtual void Exit(FiniteStateMachine& fsm) override;
	};

	class RuntimeModeState : FSMState
	{

	public:
		virtual void Enter(FiniteStateMachine& fsm)  override;
		virtual void Update(FiniteStateMachine& fsm) override;
		virtual void Exit(FiniteStateMachine& fsm) override;
	private:
		XenonCompiler* compiler = nullptr;
	};

	class XenonEngineEditor
	{
	public:
		void Initialize();
		void Update();
		void Shutdown();

		bool IsEditorMode() const;
	protected:
	private:
		FiniteStateMachine m_editorStateMachine;
	};
}
