#pragma once
#include "../StateMachine.h"
class EStateWaitandSee : public State
{
private:


	
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

