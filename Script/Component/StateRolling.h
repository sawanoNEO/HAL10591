#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateRolling :  public State
{
private:
	int startup = 4;   //ローリング(回避)の無敵時間が発生するまでの時間(フレーム)
	int invincible = 47; //無敵時間
	int recovery = 18;   //ローリングの後に行動可能になるまでの時間(フレーム)
	int cnt;       //回避成立からどれだけのフレームが経ったか
	DirectX::SimpleMath::Vector3 Rolvec;   //ローリングの方向を決める変数

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

