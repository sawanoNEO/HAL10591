#pragma once
#include "StateMachine.h"

class StateAttack : public State
{
private:
	GameObject* AttackObj;
	
	int Maxcombo;           //連続で攻撃したときのモーションの数
	int combo;              //現在の最大コンボ
	int Startup;            //攻撃の前隙のフレーム数
	int ActiveFrames;       //攻撃の持続フレーム数
	int Recovery;           //攻撃の後隙のフレーム数
	int cnt;                //現在どれだけ攻撃のモーションが進んでいるか
	int Power;              //攻撃力
	float STconsumption;    //消費するスタミナの量

public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

