#include "Rigidbody.h"
#include "gameObject.h"
#include "imguimanager.h"

using namespace DirectX::SimpleMath;

void Rigidbody::Init(float mass)
{
	rp.mass = mass;
	Drug = -10.0;
	gravity = 0.98;

}

void Rigidbody::AddForce(DirectX::SimpleMath::Vector3 _force, ForceMode forceMode)
{
	const float deltaTime = 1.f / 60.f; // 経過時間
	switch (forceMode)
	{
	case ForceMode::Force:
		rp.force = rp.force + _force;
		break;
	case ForceMode::Impulse:
		rp.force = rp.force + (_force / deltaTime);
		break;
	case ForceMode::Acceleration:
		rp.force = _force * rp.mass;
		break;
	case ForceMode::VelocityChange:
		rp.force = rp.force + (_force / deltaTime) * rp.mass;
		break;
	default:
		break;
	}
}

void Rigidbody::Update()
{
	const float deltaTime = 1.f / 60.f; // 経過時間。固定フレームレートがよい。
	Vector3 friction = -rp.velocity * Drug;
	Vector3 position = m_GameObject->GetPosition();
	Acc = rp.force / rp.mass;
	rp.velocity.y -= rp.mass * gravity;
	rp.velocity = rp.velocity + (Acc - friction) * deltaTime;     // 速度を更新
	//rp.velocity = rp.velocity + rp.force * deltaTime / rp.mass; // 速度を更新
	position = position + rp.velocity * deltaTime; // オブジェクトの位置を更新
	rp.force = Vector3(0.f, 0.f, 0.f); // 放置すると一生加速するので0に戻す
	m_GameObject->SetPosition(position);
}

void Rigidbody::Draw()
{
	ImGui::Begin("RigidBody");
	ImGui::Text("Velocity");
	ImGui::Text("x=%f\n",GetVelocity().x);
	ImGui::Text("y=%f\n",GetVelocity().y);
	ImGui::Text("z=%f\n",GetVelocity().z);
	ImGui::Text("Accel=%f\n", Acc);
	ImGui::End();
}
