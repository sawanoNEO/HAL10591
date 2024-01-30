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
	int m_AttackRate = 70;//次の行動を判定する際に、次の行動が攻撃になる確率(%)
	int m_WaitandSeeRate = 30;//次の行動を判定する際に、次の行動が様子見になる確率(%)
	Action m_Action;

public:
	State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

