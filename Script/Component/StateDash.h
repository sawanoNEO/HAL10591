#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateDash : public State
{
private:
	const char* StateName = "Dash";
	float DAccel;       //ダッシュ時の加速度
	float MaxSpeed;     //最大移動速度
	float rotationThreshold = 0.5;//回転するかどうかのズレの許容度
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

