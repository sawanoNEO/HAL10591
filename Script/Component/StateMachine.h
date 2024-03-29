#pragma once
#include "../Component/component.h"

class State:public Component
{
public:
    using Component::Component;
    virtual void Enter() = 0;    //��Ԃɓ��������̏���
    virtual void Exit() = 0;     //��Ԃ𔲂��o�����̏���
    virtual void StateUpdate() = 0; //��Ԃ��Ƃ̖����[�v����
    virtual void StateChange() = 0; //��������֐�
};

class StateMachine :public Component
{
private:
    State* m_CurrentState;//���݂̏��
    bool isEnable = true;//�X�e�[�g�}�V���̃A�N�e�B�u��

public:
    using Component::Component;

    void changeState(State* newState) //��ԑJ�ڊ֐�
    {
        m_CurrentState->Exit();
        m_CurrentState = newState;
        m_CurrentState->Enter();
    }

    void Init(State* initialState)//�ŏ��ɐݒ肷����
    {
        m_CurrentState=initialState;
        m_CurrentState->Enter();
    }

    void Update() 
    {
        if (isEnable)
        {
            m_CurrentState->StateUpdate();
            m_CurrentState->StateChange();
        }
    }
    void Draw()
    {
        m_CurrentState->Draw();
    }

    void SetEnable(bool _enable) { isEnable = _enable; }
};