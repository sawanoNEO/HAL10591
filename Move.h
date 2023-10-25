#pragma once
#include "component.h"
#include <simplemath.h>

class Move : public Component
{
private:

	float Speed;        //��{�̈ړ����x
	float Accel;        //�ʏ�̉����x
	float DAccel;       //�_�b�V�����̉����x
	float MaxSpeed;     //�ő�ړ����x
	float rotationThreshold = 0.5;//��]���邩�ǂ����̃Y���̋��e�x
	DirectX::SimpleMath::Vector3 OldPos;

public:
	using Component::Component;
	void Update();
	void Draw();

	float GetAccel() { return Accel; }
	void SetAccel(float acc) { Accel = acc; }
	float GetDAccel() { return DAccel; }
	void SetDAccel(float acc) { DAccel = acc; }
	DirectX::SimpleMath::Vector3 GetOldPos() { return OldPos; }
};

