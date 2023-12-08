#pragma once
#include "../Component/component.h"

class State:public Component
{
private:
    const char* StateName;//状態の名前
public:
    using Component::Component;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void StateUpdate() = 0;
    const char* GetStateName() { return StateName; }; 
};

class StateMachine :public Component
{
private:
    State* m_CurrentState;//現在の状態
    State* m_PastState;//前回の状態

public:
    using Component::Component;
    //StateMachine(State* initialState) : m_CurrentState(initialState) {
    //    m_CurrentState->Enter();
    //}
    void changeState(State* newState) {
        m_CurrentState->Exit();
        m_PastState = m_CurrentState;
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

    const char* GetPastStateName() { return m_PastState->GetStateName(); };
};