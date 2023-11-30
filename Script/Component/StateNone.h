#pragma once
#include "StateMachine.h"
class StateNone : public State
{
private: 
	int receptionCount; //Aボタンが押されている時間で回避orダッシュを決める。そのためのカウント。
	int reception = 10;      //Aボタンの長押しを何フレームまで許容するかの値。
public:
	using State::State; 
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

