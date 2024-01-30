#pragma once
#include "StateMachine.h"
class StateDamage : public State
{
public:
private:
	int m_Count = 0; //動作カウント
	int m_Recover = 28;//ダメージリアクションの硬直フレーム
	bool m_Hitting = false;
public:
	State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

