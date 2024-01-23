#pragma once
#include "../StateMachine.h"


class EStateChase : public State
{
private:
	enum Action
	{
		NONE,
		ATTACK,
		WAITANDSEE,
	};

	Action m_Action;

public:
	State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

