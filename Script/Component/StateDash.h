#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateDash : public State
{
private:
	const char* StateName = "Dash";
	float DAccel;       //�_�b�V�����̉����x
	float MaxSpeed;     //�ő�ړ����x
	float rotationThreshold = 0.5;//��]���邩�ǂ����̃Y���̋��e�x
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

