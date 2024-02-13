#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>
class EStateNone : public State
{
private:
	float m_Fov = 120;//視野角
	float m_Sight = 15.0;//見える距離
	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//視野範囲チェック関数。

public:
	using State::State;
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

