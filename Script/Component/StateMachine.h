#pragma once
#include "../Component/component.h"

class State:public Component
{
	public:
    using Component::Component;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void StateUpdate() = 0;
};

class StateMachine :public Component
{
private:
    State* m_CurrentState;

public:
    using Component::Component;
    StateMachine(State* initialState) : m_CurrentState(initialState) {
        m_CurrentState->Enter();
    }
    void changeState(State* newState) {
        m_CurrentState->Exit();
        m_CurrentState = newState;
        m_CurrentState->Enter();
    }

    void Init(State* initialState){
        m_CurrentState=initialState;
        m_CurrentState->Enter();
    }

    void Update() {
        m_CurrentState->StateUpdate();
    }
    void Draw() {
        m_CurrentState->Draw();
    }
};