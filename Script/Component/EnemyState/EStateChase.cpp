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
	Vector3 forward;  //自分が進む方向
	Vector3 rotation = m_GameObject->GetRotation();
	Vector3 playerpos = player->GetPosition();
	Vector3 vec = playerpos - m_GameObject->GetPosition();

	//追跡状態では常に相手(プレイヤーの方向に向き続ける)
	rotation.y = atan2(vec.x, vec.z);
	forward = XMVector3Normalize(playerpos - m_GameObject->GetPosition());
	
	m_GameObject->SetRotation(rotation);
	rb->AddForce(forward * 350.0, ForceMode::Force);

	float distance = (playerpos - m_GameObject->GetPosition()).Length();

	if (distance < 6.0)
	{
		switch (rand()%100<m_AttackRate?0:1)
		{
		case 0:
			m_AttackRate -= 10;
			m_WaitandSeeRate += 10;
			m_Action = ATTACK;
			break;
		case 1:
			m_WaitandSeeRate -= 30;
			m_AttackRate += 30;
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
	if (vec.Length() > 60.0f)
	{
		//待機状態に戻る
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
	else if (/*vec.Length() < 3.0f*/m_Action==ATTACK)
	{
		//攻撃する
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateAttack>());
	}
	else if (/*vec.Length() < 3.0f*/m_Action==WAITANDSEE)
	{
		//様子見をする
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateWaitandSee>());
	}
}

void EStateChase::Draw()
{
#if _DEBUG
	ImGui::Begin("EnemyChase");
	ImGui::Text("Attack%i", m_AttackRate);
	ImGui::Text("WAS%i", m_WaitandSeeRate);
	ImGui::End();
#endif
}
