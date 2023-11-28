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
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};