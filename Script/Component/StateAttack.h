#pragma once
#include "StateMachine.h"

class Colider;

class StateAttack : public State
{
private:
	Colider* m_AttackColider = nullptr;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

