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

		virtual void Enter(FiniteStateMachine& fsm);
		virtual void Update(FiniteStateMachine& fsm);
		virtual void Exit(FiniteStateMachine& fsm);
	protected:
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

		bool SetStates(Vector<String>& vectStateName);
		bool SetStates(const std::initializer_list<String>& list);  
		bool SetStates(Vector<int>& vectStateId);
		bool SetStates(const std::initializer_list<int>& list);

		inline int FocusStateEnum() const { return m_fsmState ? m_fsmState->m_enumState : -1; }
		inline const String& FocusStateName()
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
		FSMState* GetState(int eState);
		FSMState* GetState(const String& stateName);

		int m_enumNextState;
		int m_enumLastState;
		FSMState* m_fsmState;
		void* custom_;
		Vector<FSMState*> m_vectorStates;
		friend class FSMState;

		class OpenFSMPool
		{
		public:
			OpenFSMPool();
			~OpenFSMPool();

			bool registerState(FSMState* state);
			bool registerState(const String& stateName, int enumState = -1);
			bool registerRelation(const String& stateName, Vector<String>& vectStateName);

			FSMState* getState(int state);
			FSMState* getState(const String& stateName);
		private:
			int uid_;
			std::map<String, FSMState*> m_mapNameState;
		};
		static OpenFSMPool m_FSMPool;

	public:
		static bool RegisterState(FSMState* state)
		{
			return m_FSMPool.registerState(state);
		}
		static bool RegisterState(const String& stateName, int enumState = -1)
		{
			return m_FSMPool.registerState(stateName, enumState);
		}
		static bool RegisterRelation(const String& stateName, Vector<String>& vectStateName)
		{
			return m_FSMPool.registerRelation(stateName, vectStateName);
		}
	};
}