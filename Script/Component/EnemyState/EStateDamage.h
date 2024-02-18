#pragma once
#include "../StateMachine.h"
class EStateDamage : public State
{
private:
	int m_Count = 0; //動作カウント
	int m_Recover = 40;//ダメージリアクションの硬直フレーム
	bool m_Hitting = false;
public:
	State::State;
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

