#pragma once
#include "StateMachine.h"
class StateDeath :  public State
{
private:
	int m_DeathFrame = 116;//消滅までのフレーム
	int m_Count = 0;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

