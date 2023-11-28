#pragma once
#include "../Component/StateMachine.h"
#include <SimpleMath.h>
class StateMove :public State
{
private:
	float Speed;        //��{�̈ړ����x
	float Accel;        //�ʏ�̉����x
	float DAccel;       //�_�b�V�����̉����x
	float MaxSpeed;     //�ő�ړ����x
	float rotationThreshold = 0.5;//��]���邩�ǂ����̃Y���̋��e�x
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};