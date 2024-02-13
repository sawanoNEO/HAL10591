#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>
#include <vector>

class EStateAttack : public State
{
private:
	GameObject* m_AttackObj;
	class Slash* slash;
	std::vector<int> m_Startup;            //攻撃の前隙のフレーム数
	std::vector<int> m_ActiveFrames;       //攻撃の持続フレーム数
	std::vector<int> m_Recovery;           //攻撃の後隙のフレーム数
	std::vector<int> m_Power;              //攻撃力
	std::vector<int> m_DubleAttackRate;    //連続攻撃の確率(%)
	int m_cnt = 0;            //現在どれだけ攻撃のモーションが進んでいるか

	int m_AttackNum = 0;//何回連続で攻撃したか
	bool m_DoubleAttack = false;//連続攻撃を出したかどうか
	bool m_hit = false;       //一度の攻撃で複数ヒットした判定になるのを防ぐフラグ

	int m_SENumber;           //どの音を再生するか
public:
	using State::State;
	void Init()override;
	void Init(int _startup, int _activeframes, int _recovery, int _power, int _doubleattackrate, DirectX::SimpleMath::Vector3 _scale); 
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

