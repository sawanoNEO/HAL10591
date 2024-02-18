#pragma once
#include "../StateMachine.h"
class EStateDamage : public State
{
private:
	int m_Count = 0; //����J�E���g
	int m_Recover = 40;//�_���[�W���A�N�V�����̍d���t���[��
	bool m_Hitting = false;
public:
	State::State;
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

