#pragma once

#include "String.h"
#include <map>

namespace Algorithm
{
	class FSMState
	{
		friend class FiniteStateMachine;
	public:
		FSMState();
		FSMState(int state, const String& stateName);
		virtual ~FSMState();

		virtual void Enter(FiniteStateMachine& fsm) const;
		virtual void Update(FiniteStateMachine& fsm) const;
		virtual void Exit(FiniteStateMachine& fsm) const;
	protected:
		void NextAction(FiniteStateMachine& fsm) const;
		int m_enumState;
		String m_stateName;
		Vector<FSMState*> m_vectRelationState;
	};


	class FiniteStateMachine
	{
	public:
		FiniteStateMachine();
		virtual ~FiniteStateMachine();
		virtual void Update();
		bool EnterState(int eState);
		bool EnterState(const String& stateName);

		bool NextState(int eState);
		bool NextState(const String& stateName);
		void NextAction();

		bool SetStates(Vector<String>& vectStateName);
		bool SetStates(const std::initializer_list<String>& list);  
		bool SetStates(Vector<int>& vectStateId);
		bool SetStates(const std::initializer_list<int>& list);

		inline int focusEState() { return m_fsmState ? m_fsmState->m_enumState : 0; }
		inline const String& focusStateName()
		{
			static String empty;
			return m_fsmState ? m_fsmState->m_stateName : empty;
		}

		inline void operator=(const std::initializer_list<String>& list) { SetStates(list); }
		inline void operator=(const std::initializer_list<int>& list) { SetStates(list); }

		inline void setCustom(void* custom) { custom_ = custom; }
		template <class T>
		inline T* custom() { return dynamic_cast<T*>((T*)custom_); }

	private:
		FSMState* getState(int eState);
		FSMState* getState(const String& stateName);

		int eNextState_;
		int eLastState_;
		unsigned int actionIdx_;
		FSMState* m_fsmState;
		void* custom_;
		Vector<FSMState*> vectState_;
		friend class FSMState;

		class OpenFSMPool
		{
		public:
			OpenFSMPool();
			~OpenFSMPool();

			bool registerState(FSMState* state);
			bool registerState(const String& stateName, Vector<String>& vectActionName, int enumState = -1);
			bool registerRelation(const String& stateName, Vector<String>& vectStateName);

			FSMState* getState(int state);
			FSMState* getState(const String& stateName);
		private:
			int uid_;
			std::map<String, FSMState*> m_mapNameState;
		};
		static OpenFSMPool m_FSMPool;

	public:
		template <class T>
		static void RegisterAction(const String& actionName)
		{
			m_FSMPool.registerAction<T>(actionName);
		}
		static bool RegisterState(const String& stateName, Vector<String>& vectActionName, int enumState = -1)
		{
			return m_FSMPool.registerState(stateName, vectActionName, enumState);
		}
		static bool RegisterRelation(const String& stateName, Vector<String>& vectStateName)
		{
			return m_FSMPool.registerRelation(stateName, vectStateName);
		}
	};
}