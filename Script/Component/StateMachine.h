#pragma once
#include "../Component/component.h"

class State:public Component
{
public:
    using Component::Component;
    virtual void Enter() = 0;    //状態に入った時の処理
    virtual void Exit() = 0;     //状態を抜け出す時の処理
    virtual void StateUpdate() = 0; //状態ごとの毎ループ処理
    virtual void StateChange() = 0; //条件判定関数
};

class StateMachine :public Component
{
private:
    State* m_CurrentState;//現在の状態
    State* m_PastState;//前回の状態

public:
    using Component::Component;

    void changeState(State* newState) //状態遷移関数
    {
        m_CurrentState->Exit();
        m_PastState = m_CurrentState;
        m_CurrentState = newState;
        m_CurrentState->Enter();
    }

    void Init(State* initialState)//最初に設定する状態
    {
        m_CurrentState=initialState;
        m_CurrentState->Enter();
    }

    void Update() 
    {
        m_CurrentState->StateUpdate();
        m_CurrentState->StateChange();
    }
    void Draw()
    {
        m_CurrentState->Draw();
    }
};