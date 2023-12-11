#pragma once
#include "../Component/StateMachine.h"
#include <SimpleMath.h>
class StateMove :public State
{
private:
	float Speed;        //基本の移動速度
	float Accel;        //通常の加速度
	float DAccel;       //ダッシュ時の加速度
	float MaxSpeed;     //最大移動速度
	float rotationThreshold = 0.5;//回転するかどうかのズレの許容度
	int receptionCount; //Aボタンが押されている時間で回避orダッシュを決める。そのためのカウント。
	int reception = 20;      //Aボタンの長押しを何フレームまで許容するかの値。
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};