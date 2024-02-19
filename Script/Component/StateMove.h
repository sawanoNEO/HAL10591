#pragma once
#include "../Component/StateMachine.h"
#include <SimpleMath.h>
class StateMove :public State
{
private:
	float m_Accel;        //�ʏ�̉����x
	float m_DAccel;       //�_�b�V�����̉����x
	int m_receptionCount=0; //A�{�^����������Ă��鎞�Ԃŉ��or�_�b�V�������߂�B���̂��߂̃J�E���g�B
	int m_reception = 20;      //A�{�^���̒����������t���[���܂ŋ��e���邩�̒l�B
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};