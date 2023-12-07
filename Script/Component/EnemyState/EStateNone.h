#pragma once
#include "../StateMachine.h"
class EStateNone : public State
{
private:

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

