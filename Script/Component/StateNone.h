#pragma once
#include "StateMachine.h"
class StateNone : public State
{
public:
	using State::State; 
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

