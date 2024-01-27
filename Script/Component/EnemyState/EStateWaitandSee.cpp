#include "EStateWaitandSee.h"
#include "EStateNone.h"

#include "../Rigidbody.h"

#include "../../GameObject/player.h"
#include "../../GameObject/ColiderLooker.h"

#include "../../Scene/scene.h"

#include "../../ImGui/imguimanager.h"

#include "../../System/manager.h"

using namespace DirectX::SimpleMath;

void EStateWaitandSee::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 vec = m_GameObject->GetPosition() - player->GetPosition();//自身とプレイヤーとのベクトル
	vec.Normalize();

	m_BackPoint = vec * m_Distance;

	m_HowLong = 120;
	m_GameObject->SetAnimSpeed(-1.0f);
	m_GameObject->SetAnimName2("Walk");
	GameObject* obj = scene->AddGameObject<ColiderLooker>(1);
	obj->SetPosition(m_BackPoint);
	obj->SetScale(Vector3(1.0f, 1.0f, 1.0f));
}

void EStateWaitandSee::Exit()
{
	m_GameObject->SetAnimSpeed(1.0f);
}

void EStateWaitandSee::StateUpdate()
{
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();

	m_WalkDirection = m_GameObject->GetPosition() - m_BackPoint;
	m_WalkDirection.Normalize();
	rb->AddForce(m_WalkDirection * 200, ForceMode::Force);
}

void EStateWaitandSee::StateChange()
{
	if ((m_GameObject->GetPosition() - m_BackPoint).Length() < 0.5)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
}

void EStateWaitandSee::Draw()
{
	ImGui::Begin("WaitAndSee");
	ImGui::SliderFloat("Distance", &m_Distance, 1.0f, 10.0f);
	ImGui::End();
}
