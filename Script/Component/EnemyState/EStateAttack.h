#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateAttack : public State
{
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

