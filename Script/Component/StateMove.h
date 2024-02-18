#pragma once
#include "../Component/StateMachine.h"
#include <SimpleMath.h>
class StateMove :public State
{
private:
	float m_Accel;        //通常の加速度
	float m_DAccel;       //ダッシュ時の加速度
	int m_receptionCount=0; //Aボタンが押されている時間で回避orダッシュを決める。そのためのカウント。
	int m_reception = 20;      //Aボタンの長押しを何フレームまで許容するかの値。
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};