#pragma once
#include "component.h"
#include <SimpleMath.h>
	
    struct BoundingSphere2 {
		DirectX::SimpleMath::Vector3	center;
		float	radius;
	};

	// BOX��`
	struct AABB2 {
		DirectX::SimpleMath::Vector3	min;
		DirectX::SimpleMath::Vector3	max;
	};


	enum HitState
	{
		NORMAL = 0,
		HIT = 1,
		ESCAPE = 2,
		TOUTCH = 3,
	};

	enum Tug
	{
		DEFAULT,
		PLAYER,
		ENEMY,
		WALL,
	};

	enum Type
	{
		BOX,
	};

class Colider : public Component
{
private:
	DirectX::SimpleMath::Vector3 pos;        //�����蔻�肪��������ʒu
	DirectX::SimpleMath::Vector3 Size = {1.0f,1.0f,1.0f};       //�����蔻������f���̑傫������ǂꂾ���傫�����邩
	DirectX::SimpleMath::Vector3 Scale;
	BoundingSphere2 Sphere;                   //���^�̓����蔻��
	AABB2 aabb;                               //���̓����蔻��
	bool enable;                             //���̃R���|�[�l���g���L���ł��邩�ǂ���(false���ƕ��̂ɓ�����Ȃ�
	HitState hitstate;                       //�����蔻��̏�
	Tug tug;
	Type type;

public:
	using Component::Component;
	bool CollisionSphere(BoundingSphere2 p1, BoundingSphere2 p2);

	void Init(Tug t,DirectX::SimpleMath::Vector3 size);
	void Init(Type,Tug t,DirectX::SimpleMath::Vector3 size);
	void Update();

	// AABB
	Colider CollisionAABB(AABB2 p1,Colider* p2);
	Colider CollisionAABBRight(AABB2 p1, Colider* p2);
	Colider CollisionAABBLeft(AABB2 p1, Colider* p2);
	Colider CollisionAABBTop(AABB2 p1, Colider* p2);
	Colider CollisionAABBBottom(AABB2 p1, Colider* p2);
	Colider CollisionAABBHead(AABB2 p1, Colider* p2);
	Colider CollisionAABB2D(AABB2 p1, Colider* p2);

	AABB2 GetAABB() { return aabb; }
	Tug GetTug() { return tug; }

	// make AABB
	//AABB SetAABB(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);
	AABB2 SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);



};

