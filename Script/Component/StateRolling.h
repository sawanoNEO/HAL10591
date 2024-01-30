#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateRolling :  public State
{
private:
	int m_Startup = 4;   //ローリング(回避)の無敵時間が発生するまでの時間(フレーム)
	int m_Invincible = 47; //無敵時間
	int m_Recovery = 18;   //ローリングの後に行動可能になるまでの時間(フレーム)
	int m_Count;       //回避成立からどれだけのフレームが経ったか

	bool m_KeepABurron = false;//状態遷移後からAボタンが押しっぱなしかどうか

	DirectX::SimpleMath::Vector3 m_Rolvec;   //ローリングの方向を決める変数

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

