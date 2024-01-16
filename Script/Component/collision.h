#pragma once

#include	<simplemath.h>

// ‹…’è‹`
struct BoundingSphere {
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// BOX’è‹`
struct AABB {
	DirectX::SimpleMath::Vector3	min;
	DirectX::SimpleMath::Vector3	max;
};

// ‹…
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB
bool CollisionAABB(AABB p1, AABB p2);
bool CollisionAABBRight(AABB p1, AABB p2);
bool CollisionAABBLeft(AABB p1, AABB p2);
bool CollisionAABBTop(AABB p1, AABB p2);
bool CollisionAABBBottom(AABB p1, AABB p2);
bool CollisionAABBHead(AABB p1, AABB p2);
bool CollisionAABB2D(AABB p1, AABB p2);

// make AABB
//AABB SetAABB(DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);
AABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);
