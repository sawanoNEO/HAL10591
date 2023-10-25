#include "Colider.h"
#include "manager.h"

using namespace DirectX::SimpleMath;

// 球
bool Colider:: CollisionSphere(BoundingSphere2 p1, BoundingSphere2 p2)
{

	float length = (p1.center - p2.center).Length();

	if (p1.radius + p2.radius > length) {
		return false;
	}

	return true;
}

void Colider::Init(Tug t,Vector3 size)
{
	tug = t;
	Size = size;
	enable = true;
	type = BOX;
}

void Colider::Init(Type ty, Tug t, DirectX::SimpleMath::Vector3 size)
{
	type = ty;
	tug = t;
	Size = size;
}

void Colider::Update()
{
	if (enable)//Coliderをオンオフ切り替えれるようにしている
	{
		pos = m_GameObject->GetPosition();
		Scale = m_GameObject->GetScale();
		switch (type)
		{
		case BOX:
			aabb = SetAABB(pos, fabs(Scale.x * Size.x), fabs(Scale.y * Size.y), fabs(Scale.z * Size.z));
			break;
		default:
			break;
		}
	}

}

// AABB
Colider Colider:: CollisionAABB(AABB2 p1, Colider* p2) {

	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	return *p2;
}

Colider Colider::CollisionAABBRight(AABB2 p1, Colider* p2)
{

	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	if (p1.max.x < p2aabb.min.x)
	{
		return nullptr;
	}

	if (p1.max.x > p2aabb.max.x)
	{
		return nullptr;
	}
	return *p2;
}

Colider Colider::CollisionAABBLeft(AABB2 p1, Colider* p2)
{
	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x)
	{
		return nullptr;
	}
	if (p1.min.x < p2aabb.min.x)
	{
		return nullptr;
	}
	return *p2;
}

Colider Colider::CollisionAABBTop(AABB2 p1, Colider* p2)
{
	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	if (p1.max.z < p2aabb.min.z)
	{
		return nullptr;
	}

	if (p1.max.z > p2aabb.max.z)
	{
		return nullptr;
	}
	return *p2;
}

Colider Colider::CollisionAABBBottom(AABB2 p1, Colider* p2)
{
	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	/*if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}*/

	//if (p1.min.z < p2aabb.max.z)
	//{
	//	return nullptr;
	//}

	if (p1.min.z < p2aabb.min.z)
	{
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z)
	{
		return nullptr;
	}
	return *p2;
}

Colider Colider::CollisionAABBHead(AABB2 p1, Colider* p2)
{
	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}

	// Y座標
	if (p1.max.y < p2aabb.min.y) {
		return nullptr;
	}

	if (p1.min.y > p2aabb.max.y) {
		return nullptr;
	}

	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	if (p1.max.y < p2aabb.max.y)
	{
		return nullptr;
	}
	return *p2;
}

Colider Colider::CollisionAABB2D(AABB2 p1, Colider* p2)
{
	AABB2 p2aabb = p2->GetAABB();

	// X座標
	if (p1.max.x < p2aabb.min.x) {
		return nullptr;
	}

	if (p1.min.x > p2aabb.max.x) {
		return nullptr;
	}


	// Z座標
	if (p1.max.z < p2aabb.min.z) {
		return nullptr;
	}

	if (p1.min.z > p2aabb.max.z) {
		return nullptr;
	}

	return *p2;
}


AABB2 Colider::SetAABB(Vector3 centerposition, float width, int height, int depth) 
{
	if (type == BOX)
	{
		AABB2 aabb2{};

		width = width;
		height = height;
		depth = depth;

		width = fabs(width);
		height = fabs(height);
		depth = fabs(depth);

		aabb2.min.x = centerposition.x - width / 2.0f;
		aabb2.min.y = centerposition.y - height / 2.0f;
		aabb2.min.z = centerposition.z - depth / 2.0f;

		aabb2.max.x = centerposition.x + width / 2.0f;
		aabb2.max.y = centerposition.y + height / 2.0f;
		aabb2.max.z = centerposition.z + depth / 2.0f;

		aabb = aabb2;

		return aabb2;
	}
}

//
//AABB2  Colider::SetAABB(Vector3 centerposition, float width, int height, int depth) {
//
//	AABB2 aabb{};
//
//	width = width;
//	height = height;
//	depth = depth;
//
//	width = fabs(width);
//	height = fabs(height);
//	depth = fabs(depth);
//
//	aabb.min.x = centerposition.x - width / 2.0f;
//	aabb.min.y = centerposition.y - height / 2.0f;
//	aabb.min.z = centerposition.z - depth / 2.0f;
//
//	aabb.max.x = centerposition.x + width / 2.0f;
//	aabb.max.y = centerposition.y + height / 2.0f;
//	aabb.max.z = centerposition.z + depth / 2.0f;
//
//	return aabb;
//}