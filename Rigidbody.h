#pragma once
#include "component.h"
#include <SimpleMath.h>

enum class ForceMode
{
	Force,
    Acceleration,
	Impulse,
	VelocityChange,
};

class RigidbodyParameter
{
public:
	float mass; // �I�u�W�F�N�g�̎���
	DirectX::SimpleMath::Vector3 velocity; // �I�u�W�F�N�g�̌��݂̑��x
	DirectX::SimpleMath::Vector3 force; // �I�u�W�F�N�g�Ɍ��݉������Ă����
};

class Rigidbody : public Component
{
private :
	RigidbodyParameter rp;
	float Drug;
	float gravity;
	DirectX::SimpleMath::Vector3 Acc;
public:
	using Component::Component;
	void Init(float);
	void AddForce(DirectX::SimpleMath::Vector3 _force, ForceMode forceMode);
	void Update();
	void Draw();

	DirectX::SimpleMath::Vector3 GetVelocity() { return rp.velocity; }
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { rp.velocity = vel; }
	DirectX::SimpleMath::Vector3 GetForce() { return rp.force; }
	void SetForce(DirectX::SimpleMath::Vector3 force) { rp.force = force; }
	float GetMass() { return rp.mass; }
	void SetMass(float mass) { rp.mass = mass; }
	DirectX::SimpleMath::Vector3 GetAccel() { return Acc; }
	void SetAccel(DirectX::SimpleMath::Vector3 acc) { Acc = acc; }

};

