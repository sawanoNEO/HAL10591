#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateAttack : public State
{
private:
	GameObject* AttackObj;
	int Startup;            //攻撃の前隙のフレーム数
	int ActiveFrames;       //攻撃の持続フレーム数
	int Recovery;           //攻撃の後隙のフレーム数
	int cnt = 0;                //現在どれだけ攻撃のモーションが進んでいるか
	int Power;              //攻撃力

	bool hit = false;       //一度の攻撃で複数ヒットした判定になるのを防ぐフラグ
public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

