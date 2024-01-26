#pragma once
#include "../Component/component.h"
#include <SimpleMath.h>
#include <list>
	
    struct BoundingSphere2 {
		DirectX::SimpleMath::Vector3	center;
		float	radius;
	};

	// BOX定義
	struct AABB2 {
		DirectX::SimpleMath::Vector3	min;
		DirectX::SimpleMath::Vector3	max;
	};


	enum HitDirection
	{
		TOP = 0,
		BOTTOM = 1,
		RIGHT = 2,
		LEFT = 3,
		HEAD = 4,
		HIT = 5,
		HITDIRMAX = 6
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

class Box;
class ColiderLooker;

class Colider : public Component
{
private:
	DirectX::SimpleMath::Vector3 pos;        //当たり判定が発生する位置
	DirectX::SimpleMath::Vector3 Size = {1.0f,1.0f,1.0f};       //当たり判定をモデルの大きさからどれだけ大きくするか
	DirectX::SimpleMath::Vector3 Scale;
	BoundingSphere2 Sphere;                   //球型の当たり判定
	AABB2 aabb;                               //□の当たり判定
	bool enable;                             //このコンポーネントが有効であるかどうか(falseだと物体に当たらない
	Tug tug;
	Type type;
	int a = 0;
	ColiderLooker* box = nullptr;
public:
	using Component::Component;
	bool CollisionSphere(BoundingSphere2 p1, BoundingSphere2 p2);

	void Init(Tug t,DirectX::SimpleMath::Vector3 size);
	void Init(Type,Tug t,DirectX::SimpleMath::Vector3 size);
	void Update();
	void Draw();

	// AABB
	Colider* CollisionAABB(AABB2 p1,Colider* p2);
	Colider* CollisionAABBRight(AABB2 p1, Colider* p2);
	Colider* CollisionAABBLeft(AABB2 p1, Colider* p2);
	Colider* CollisionAABBTop(AABB2 p1, Colider* p2);
	Colider* CollisionAABBBottom(AABB2 p1, Colider* p2);
	Colider* CollisionAABBHead(AABB2 p1, Colider* p2);
	Colider* CollisionAABB2D(AABB2 p1, Colider* p2);

	std::array<std::list<Colider*>,HITDIRMAX> GetAllHitColiders();

	AABB2 GetAABB() { return aabb; }
	Tug GetTug() { return tug; }

	// make AABB
	//AABB SetAABB(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);
	AABB2 SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);



};

