#pragma once
#include "StateMachine.h"
class StateNone : public State
{
private: 
	int receptionCount; //A�{�^����������Ă��鎞�Ԃŉ��or�_�b�V�������߂�B���̂��߂̃J�E���g�B
	int reception = 10;      //A�{�^���̒����������t���[���܂ŋ��e���邩�̒l�B
public:
	using State::State; 
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

