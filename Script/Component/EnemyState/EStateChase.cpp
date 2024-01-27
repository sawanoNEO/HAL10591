#include "EStateChase.h"
#include "EStateNone.h"
#include "EStateAttack.h"
#include "EStateWaitAndSee.h"

#include "../Rigidbody.h"

#include "../../System/manager.h"
#include "../../Scene/scene.h"

#include "../../GameObject/enemy.h"
#include "../../GameObject/player.h"

#include "../../ImGui/imguimanager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EStateChase::Enter()
{
	m_GameObject->SetAnimName2("Walk");
	m_Action = NONE;
}

void EStateChase::Exit()
{
}

void EStateChase::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (!player)
	{
		return;
	}
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();
	Vector3 forward = m_GameObject->GetForward();
	Vector3 rotation = m_GameObject->GetRotation();
	Vector3 playerpos = player->GetPosition();
	Vector3 vec = playerpos - m_GameObject->GetPosition();

	rotation.y = atan2(vec.x, vec.z);
	forward = XMVector3Normalize(player->GetPosition() - m_GameObject->GetPosition());
	
	m_GameObject->SetRotation(rotation);
	rb->AddForce(forward * 200.0, ForceMode::Force);

	float distance = (playerpos - m_GameObject->GetPosition()).Length();

	if (distance < 5.0)
	{
		switch (rand()%2)
		{
		case 0:
			m_Action = ATTACK;
			break;
		case 1:
			m_Action = WAITANDSEE;
			break;
		default:
			break;
		}
	}
}

void EStateChase::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (!player)
	{
		return;
	}
	Vector3 vec = player->GetPosition() - m_GameObject->GetPosition();
	if (vec.Length() > 20.0f)
	{
		//‘Ò‹@ó‘Ô‚É–ß‚é
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
	else if (/*vec.Length() < 3.0f*/m_Action==ATTACK)
	{
		//UŒ‚‚·‚é
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateAttack>());
	}
	else if (/*vec.Length() < 3.0f*/m_Action==WAITANDSEE)
	{
		//UŒ‚‚·‚é
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateWaitandSee>());
	}
}

void EStateChase::Draw()
{
	ImGui::Begin("EnemyChase");
	ImGui::End();
}
