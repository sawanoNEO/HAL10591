#pragma once
#include "../StateMachine.h"
class EStateChase : public State
{
public:
	State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

