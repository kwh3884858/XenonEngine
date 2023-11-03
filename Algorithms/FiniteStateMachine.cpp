#include "FiniteStateMachine.h"


#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

namespace open
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
        assert(m_enumState != 0);
        fsm.actionIdx_ = 0;
        if (fsm.actionIdx_ >= vectAction_.Size())
            return;

        vectAction_[fsm.actionIdx_]->Enter(fsm);
    }

    void FSMState::Update(FiniteStateMachine& fsm) const
    {
        if (fsm.actionIdx_ >= vectAction_.Size())
        {
            if (!m_vectRelationState.empty())
            {
                fsm.NextState(m_vectRelationState[0]->m_enumState);
                return;
            }
            assert(false);
            return;
        }
        vectAction_[fsm.actionIdx_]->Update(fsm);
    }

    void FSMState::exit(FiniteStateMachine& fsm) const
    {
        if (fsm.actionIdx_ < vectAction_.Size())
        {
            vectAction_[fsm.actionIdx_]->exit(fsm);
        }
    }

    void FSMState::NextAction(FiniteStateMachine& fsm) const
    {
        if (fsm.actionIdx_ < vectAction_.Size())
        {
            vectAction_[fsm.actionIdx_]->exit(fsm);
        }
        fsm.actionIdx_++;
        if (fsm.actionIdx_ < vectAction_.Size())
        {
            vectAction_[fsm.actionIdx_]->Enter(fsm);
        }
        else
        {
            if (!m_vectRelationState.empty())
            {
                fsm.NextState(m_vectRelationState[0]->m_enumState);
                return;
            }
            assert(false);
            return;
        }
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
            m_fsmState->exit(*this);
            eLastState_ = m_fsmState->m_enumState;
        }
        m_fsmState = getState(eState);
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
            m_fsmState->exit(*this);
            eLastState_ = m_fsmState->m_enumState;
        }
        m_fsmState = getState(stateName);
        if (!m_fsmState)
        {
            return false;
        }
        m_fsmState->Enter(*this);
        return true;
    }

    bool FiniteStateMachine::NextState(int eState)
    {
        FSMState* state = getState(eState);
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
        FSMState* state = getState(stateName);
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
            if (vectState_.empty())
            {
                return;
            }
            NextState(vectState_[0]->m_enumState);
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

    bool FiniteStateMachine::SetStates(std::vector<String>& vectStateName)
    {
        vectState_.clear();
        FSMState* state = 0;
        for (Size_t i = 0; i < vectStateName.Size(); ++i)
        {
            state = m_FSMPool.getState(vectStateName[i]);
            if (!state)
            {
                vectState_.clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            vectState_.push_back(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(const std::initializer_list<String>& list)
    {
        vectState_.clear();
        FSMState* state = 0;
        std::initializer_list<String>::const_iterator iter = list.begin();
        for (; iter != list.end(); iter++)
        {
            state = m_FSMPool.getState(*iter);
            if (!state)
            {
                vectState_.clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            vectState_.push_back(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(std::vector<int>& vectStateId)
    {
        vectState_.clear();
        FSMState* state = 0;
        for (Size_t i = 0; i < vectStateId.Size(); ++i)
        {
            state = m_FSMPool.getState(vectStateId[i]);
            if (!state)
            {
                vectState_.clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            vectState_.push_back(state);
        }
        return true;
    }

    bool FiniteStateMachine::SetStates(const std::initializer_list<int>& list)
    {
        vectState_.clear();
        FSMState* state = 0;
        std::initializer_list<int>::const_iterator iter = list.begin();
        for (; iter != list.end(); iter++)
        {
            state = m_FSMPool.getState(*iter);
            if (!state)
            {
                vectState_.clear();
                assert(false);
                return false;
            }
            assert(state->m_enumState != 0);
            vectState_.push_back(state);
        }
        return true;
    }

    FSMState* FiniteStateMachine::getState(int eState)
    {
        for (Size_t i = 0; i < vectState_.Size(); ++i)
        {
            if (vectState_[i]->m_enumState == eState)
                return vectState_[i];
        }
        return 0;
    }

    FSMState* FiniteStateMachine::getState(const String& stateName)
    {
        for (Size_t i = 0; i < vectState_.Size(); ++i)
        {
            if (vectState_[i]->m_stateName == stateName)
                return vectState_[i];
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
        std::map<String, OpenFSMAction*>::iterator iter2 = mapNameAction_.begin();
        for (; iter2 != mapNameAction_.end(); iter2++)
        {
            delete iter2->second;
        }
        mapNameAction_.clear();
    }

    bool FiniteStateMachine::OpenFSMPool::registerAction(OpenFSMAction* action)
    {
        if (!action)
        {
            assert(false);
            return false;
        }
        const String& actionName = action->actionName_;
        std::map<String, OpenFSMAction*>::iterator iter = mapNameAction_.find(actionName);
        if (iter != mapNameAction_.end())
        {
            printf("OpenFSMPool::registerAction actionName [%s] is exist!\n", actionName.c_str());
            assert(false);
            return false;
        }
        mapNameAction_[actionName] = action;
        return true;
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
            printf("OpenFSMPool::registerState stateName [%s] is exist!\n", stateName.c_str());
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

    bool FiniteStateMachine::OpenFSMPool::registerState(const String& stateName, std::vector<String>& vectActionName, int eState)
    {
        std::map<String, FSMState*>::iterator iter = m_mapNameState.find(stateName);
        if (iter != m_mapNameState.end())
        {
            printf("OpenFSMPool::registerState stateName [%s] is exist!\n", stateName.c_str());
            assert(false);
            return false;
        }
        if (eState <= 0)
        {
            eState = -uid_++;
        }
        FSMState* state = new FSMState(eState, stateName);
        OpenFSMAction* action = 0;
        for (Size_t i = 0; i < vectActionName.Size(); ++i)
        {
            action = m_FSMPool.getAction(vectActionName[i]);
            if (!action)
            {
                delete state;
                printf("OpenFSMPool::registerState actionName [%s] is not exist!\n", vectActionName[i].c_str());
                assert(false);
                return false;
            }
            state->vectAction_.push_back(action);
        }
        assert(state->m_enumState != 0);
        m_mapNameState[stateName] = state;
        return true;
    }

    bool FiniteStateMachine::OpenFSMPool::registerRelation(const String& stateName, std::vector<String>& vectStateName)
    {
        if (vectStateName.empty())
        {
            printf("OpenFSMPool::registerRelation[%s] vectStateName is empty!\n", stateName.c_str());
            assert(false);
            return false;
        }
        FSMState* focusState = getState(stateName);
        if (!focusState)
        {
            printf("OpenFSMPool::registerRelation[%s] state is not exist!\n", stateName.c_str());
            assert(false);
            return false;
        }
        if (!focusState->m_vectRelationState.empty())
        {
            printf("OpenFSMPool::registerRelation[%s] state had relation!\n", stateName.c_str());
            assert(false);
            return false;
        }
        std::vector<FSMState*> vectState;
        FSMState* state = 0;
        for (Size_t i = 0; i < vectStateName.Size(); ++i)
        {
            state = getState(vectStateName[i]);
            if (!state)
            {
                printf("OpenFSMPool::registerRelation stateName [%s] is not exist!\n", vectStateName[i].c_str());
                assert(false);
                return false;
            }
            vectState.push_back(state);
        }
        focusState->m_vectRelationState = vectState;
        return true;
    }

    OpenFSMAction* FiniteStateMachine::OpenFSMPool::getAction(const String& actionName)
    {
        std::map<String, OpenFSMAction*>::iterator iter = mapNameAction_.find(actionName);
        if (iter != mapNameAction_.end())
        {
            return iter->second;
        }
        return 0;
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