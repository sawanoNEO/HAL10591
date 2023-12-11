#pragma once
#include "../StateMachine.h"
class EStateDamage : public State
{
private:
	int m_Count = 0; //動作カウント
public:
	State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

