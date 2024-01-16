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
	int receptionCount; //A�{�^����������Ă��鎞�Ԃŉ��or�_�b�V�������߂�B���̂��߂̃J�E���g�B
	int reception = 20;      //A�{�^���̒����������t���[���܂ŋ��e���邩�̒l�B
	DirectX::SimpleMath::Vector3 OldPos;
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};