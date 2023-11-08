#include "FiniteStateMachine.h"


#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

namespace Algorithm
{
    //OpenFSMState
    FSMState::FSMState()
        :m_enumState(-1)
        , m_stateName("UnkownState")
    {
        assert(false);
    }

    FSMState::FSMState(int eState, const String& stateName)
        :m_enumState(eState)
        , m_stateName(stateName)
    {
    }

    FSMState::~FSMState()
    {
    }

    void FSMState::Enter(FiniteStateMachine& fsm) const
    {

    }

    void FSMState::Update(FiniteStateMachine& fsm) const
    {
		if (!m_vectRelationState.Empty())
		{
			fsm.NextState(m_vectRelationState[0]->m_enumState);
			return;
		}
		assert(false);
		return;
    }

    void FSMState::Exit(FiniteStateMachine& fsm) const
    {

    }

    void FSMState::NextAction(FiniteStateMachine& fsm) const
    {
		if (!m_vectRelationState.Empty())
		{
			fsm.NextState(m_vectRelationState[0]->m_enumState);
			return;
		}
		assert(false);
		return;
    }


    //OpenFSM
    FiniteStateMachine::OpenFSMPool FiniteStateMachine::m_FSMPool;
    FiniteStateMachine::FiniteStateMachine()
        :eNextState_(0)
        , eLastState_(0)
        , m_fsmState(0)
    {
    }

    FiniteStateMachine::~FiniteStateMachine()
    {
    }

    bool FiniteStateMachine::EnterState(int eState)
    {
        if (m_fsmState)
        {
            m_fsmState->Exit(*this);
            eLastState_ = m_fsmState->m_enumState;
        }
        m_fsmState = GetState(eState);
        if (!m_fsmState)
        {
            return false;
        }
        m_fsmState->Enter(*this);
        return true;
    }

    bool FiniteStateMachine::EnterState(const String& stateName)
    {
        if (m_fsmState)
        {
            m_fsmState->Exit(*this);
            eLastState_ = m_fsmState->m_enumState;
        }
        m_fsmState = GetState(stateName);
        if (!m_fsmState)
        {
            return false;
        }
        m_fsmState->Enter(*this);
        return true;
    }

    bool FiniteStateMachine::NextState(int eState)
    {
        FSMState* state = GetState(eState);
        if (!state)
        {
            return false;
        }
        assert(state->m_enumState == eState);
        eNextState_ = eState;
        return true;
    }

    bool FiniteStateMachine::NextState(const String& stateName)
    {
        FSMState* state = GetState(stateName);
        if (!state)
        {
            return false;
        }
        assert(state->m_stateName == stateName);
        eNextState_ = state->m_enumState;
        return true;
    }

    void FiniteStateMachine::NextAction()
    {
        if (!m_fsmState)
        {
            if (m_vectorStates.Empty())
            {
                return;
            }
            NextState(m_vectorStates[0]->m_enumState);
            return;
        }
        m_fsmState->NextAction(*this);
    }

    void FiniteStateMachine::Update()
    {
        eNextState_ = 0;
        if (m_fsmState)
        {
            m_fsmState->Update(*this);
        }
        while (eNextState_ != 0)
        {
            int state = eNextState_;
            eNextState_ = 0;
            EnterState(state);
            if (m_fsmState)
            {
                m_fsmState->Update(*this);
            }
        }
    }

	bool FiniteStateMachine::SetStates(Vector<String>& vectStateName)
    {
        m_vectorStates.Clear();
        FSMState* state = 0;
        for (int i = 0; i < vectStateName.Count(); ++i)
        {
            state = m_FSMPool.getState(vectStateName[i]);
            if (!state)
            {
                m_vectorStates.Clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            m_vectorStates.Add(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(const std::initializer_list<String>& list)
    {
        m_vectorStates.Clear();
        FSMState* state = 0;
        std::initializer_list<String>::const_iterator iter = list.begin();
        for (; iter != list.end(); iter++)
        {
            state = m_FSMPool.getState(*iter);
            if (!state)
            {
                m_vectorStates.Clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            m_vectorStates.Add(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(Vector<int>& vectStateId)
    {
        m_vectorStates.Clear();
        FSMState* state = 0;
		for (int i = 0; i < vectStateId.Count(); ++i)
        {
            state = m_FSMPool.getState(vectStateId[i]);
            if (!state)
            {
                m_vectorStates.Clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            m_vectorStates.Add(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(const std::initializer_list<int>& list)
    {
        m_vectorStates.Clear();
        FSMState* state = 0;
        std::initializer_list<int>::const_iterator iter = list.begin();
        for (; iter != list.end(); iter++)
        {
            state = m_FSMPool.getState(*iter);
            if (!state)
            {
                m_vectorStates.Clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            m_vectorStates.Add(state);
        }
        return true;
    }

    FSMState* FiniteStateMachine::GetState(int eState)
    {
        for (int i = 0; i < m_vectorStates.Count(); ++i)
        {
            if (m_vectorStates[i]->m_enumState == eState)
                return m_vectorStates[i];
        }
        return 0;
    }

    FSMState* FiniteStateMachine::GetState(const String& stateName)
    {
        for (int i = 0; i < m_vectorStates.Count(); ++i)
        {
            if (m_vectorStates[i]->m_stateName == stateName)
                return m_vectorStates[i];
        }
        return 0;
    }


    //OpenFSMPool
    FiniteStateMachine::OpenFSMPool::OpenFSMPool()
        :uid_(1)
    {
    }

    FiniteStateMachine::OpenFSMPool::~OpenFSMPool()
    {
        std::map<String, FSMState*>::iterator iter1 = m_mapNameState.begin();
        for (; iter1 != m_mapNameState.end(); iter1++)
        {
            delete iter1->second;
        }
        m_mapNameState.clear();
    }

    bool FiniteStateMachine::OpenFSMPool::registerState(FSMState* state)
    {
        if (!state)
        {
            assert(false);
            return false;
        }
        const String& stateName = state->m_stateName;
        std::map<String, FSMState*>::iterator iter = m_mapNameState.find(stateName);
        if (iter != m_mapNameState.end())
        {
            printf("OpenFSMPool::registerState stateName [%s] is exist!\n", stateName.CString());
            assert(false);
            return false;
        }
        if (state->m_enumState <= 0)
        {
            state->m_enumState = -uid_++;
        }
        assert(state->m_enumState != 0);
        m_mapNameState[stateName] = state;
        return true;
    }

    bool FiniteStateMachine::OpenFSMPool::registerState(const String& stateName, Vector<String>& vectActionName, int eState)
    {
        std::map<String, FSMState*>::iterator iter = m_mapNameState.find(stateName);
        if (iter != m_mapNameState.end())
        {
            printf("OpenFSMPool::registerState stateName [%s] is exist!\n", stateName.CString());
            assert(false);
            return false;
        }
        if (eState <= 0)
        {
            eState = -uid_++;
        }
        FSMState* state = new FSMState(eState, stateName);

        assert(state->m_enumState != 0);
        m_mapNameState[stateName] = state;
        return true;
    }

    bool FiniteStateMachine::OpenFSMPool::registerRelation(const String& stateName, Vector<String>& vectStateName)
    {
        if (vectStateName.Empty())
        {
            printf("OpenFSMPool::registerRelation[%s] vectStateName is empty!\n", stateName.CString());
            assert(false);
            return false;
        }
        FSMState* focusState = getState(stateName);
        if (!focusState)
        {
            printf("OpenFSMPool::registerRelation[%s] state is not exist!\n", stateName.CString());
            assert(false);
            return false;
        }
        if (!focusState->m_vectRelationState.Empty())
        {
            printf("OpenFSMPool::registerRelation[%s] state had relation!\n", stateName.CString());
            assert(false);
            return false;
        }
        Vector<FSMState*> vectState;
        FSMState* state = 0;
        for (int i = 0; i < vectStateName.Count(); ++i)
        {
            state = getState(vectStateName[i]);
            if (!state)
            {
                printf("OpenFSMPool::registerRelation stateName [%s] is not exist!\n", vectStateName[i].CString());
                assert(false);
                return false;
            }
            vectState.Add(state);
        }
        focusState->m_vectRelationState = vectState;
        return true;
    }

    FSMState* FiniteStateMachine::OpenFSMPool::getState(const String& stateName)
    {
        std::map<String, FSMState*>::iterator iter = m_mapNameState.find(stateName);
        if (iter != m_mapNameState.end())
        {
            return iter->second;
        }
        return 0;
    }

    FSMState* FiniteStateMachine::OpenFSMPool::getState(int eState)
    {
        FSMState* state = 0;
        std::map<String, FSMState*>::iterator iter = m_mapNameState.begin();
        for (; iter != m_mapNameState.end(); iter++)
        {
            state = iter->second;
            if (state->m_enumState >= 0 && state->m_enumState == eState)
            {
                return state;
            }
        }
        return 0;
    }

};