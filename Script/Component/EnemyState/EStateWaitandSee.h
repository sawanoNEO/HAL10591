#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateWaitandSee : public State
{
private:

	int m_HowLong;//様子見をする時間(フレーム)
	int m_Count;//経過フレーム
	float m_Distance=5.0f; //m_BackPointを決定する際の距離を決める
	DirectX::SimpleMath::Vector3 m_BackPoint;//プレイヤーからある程度離れた距離の点
	DirectX::SimpleMath::Vector3 m_WalkDirection;//歩く方向
	
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

