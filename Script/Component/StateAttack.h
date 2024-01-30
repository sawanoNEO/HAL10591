#pragma once
#include "StateMachine.h"

class StateAttack : public State
{
private:
	GameObject* AttackObj;
	
	class Slash* slash;

	int m_Maxcombo;           //連続で攻撃したときのモーションの数
	int m_combo;              //現在の最大コンボ
	int m_Startup;            //攻撃の前隙のフレーム数
	int m_ActiveFrames;       //攻撃の持続フレーム数
	int m_Recovery;           //攻撃の後隙のフレーム数
	int m_Count;                //現在どれだけ攻撃のモーションが進んでいるか
	int m_Power;              //攻撃力
	float m_Staminaconsumption;    //消費するスタミナの量

public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

