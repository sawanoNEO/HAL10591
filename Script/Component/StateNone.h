#pragma once
#include "StateMachine.h"
class StateNone : public State
{
private: 
	int m_ReceptionCount=0; //Aボタンが押されている時間で回避orダッシュを決める。そのためのカウント。
	int m_Reception = 10;      //Aボタンの長押しを何フレームまで許容するかの値。
	bool m_KeepAButton = false;
public:
	using State::State; 
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

