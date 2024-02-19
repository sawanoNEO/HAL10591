#pragma once
#include "../Component/component.h"

class State:public Component
{
public:
    using Component::Component;
    virtual void Enter() = 0;    //ó‘Ô‚É“ü‚Á‚½Žž‚Ìˆ—
    virtual void Exit() = 0;     //ó‘Ô‚ð”²‚¯o‚·Žž‚Ìˆ—
    virtual void StateUpdate() = 0; //ó‘Ô‚²‚Æ‚Ì–ˆƒ‹[ƒvˆ—
    virtual void StateChange() = 0; //ðŒ”»’èŠÖ”
};

class StateMachine :public Component
{
private:
    State* m_CurrentState;//Œ»Ý‚Ìó‘Ô

public:
    using Component::Component;

    void changeState(State* newState) //ó‘Ô‘JˆÚŠÖ”
    {
        m_CurrentState->Exit();
        m_CurrentState = newState;
        m_CurrentState->Enter();
    }

    void Init(State* initialState)//Å‰‚ÉÝ’è‚·‚éó‘Ô
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