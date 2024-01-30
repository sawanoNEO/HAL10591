#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

//プレイヤーと一定の距離を保った後に様子見を行う状態
class EStateWaitandSee : public State
{
private:

	int m_HowLong;//様子見をする時間(フレーム)
	int m_Count=0;//様子見を開始してからの経過フレーム
	float m_Distance=5.0f; //m_BackPointを決定する際の距離を決める
	DirectX::SimpleMath::Vector3 m_BackPoint;//プレイヤーからある程度離れた距離の点
	DirectX::SimpleMath::Vector3 m_WalkVec;//歩く方向
	int m_Direction;//様子見の際に左右どちらに動くかを決定する変数
	bool m_SideMoveflg = false;//後ろ移動を終え、横移動をするかどうか
	int m_RandUpperLimit = 90;//様子見をする時間を決定する乱数の上限
	int m_RandLowerLimit = 40;//様子見をする時間を決定する乱数の下限
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

