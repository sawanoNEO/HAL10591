#pragma once
#include "StateMachine.h"
class StateNone : public State
{
private: 
	int m_ReceptionCount=0; //A�{�^����������Ă��鎞�Ԃŉ��or�_�b�V�������߂�B���̂��߂̃J�E���g�B
	int m_Reception = 10;      //A�{�^���̒����������t���[���܂ŋ��e���邩�̒l�B
	bool m_KeepAButton = false;
public:
	using State::State; 
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

