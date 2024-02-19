#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateDash : public State
{
private:
	float m_DAccel;       //�_�b�V�����̉����x
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};