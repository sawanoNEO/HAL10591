#include "EStateDamage.h"
#include "EStateNone.h"
#include "EStateChase.h"

#include "../../System/manager.h"
#include "../../Scene/scene.h"

#include "../../GameObject/gameObject.h"
#include "../../GameObject/player.h"

#include "../../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void EStateDamage::Enter()
{
	if (m_Hitting == false)
	{
		m_GameObject->SetAnimName2("Impact");
		m_Count = 0;
	}
	m_Hitting = true;
}

void EStateDamage::Exit()
{
	if (m_Count > m_Recover)
	{
		m_Hitting = false;
	}
}

void EStateDamage::StateUpdate()
{
	m_Count++;
}

void EStateDamage::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 pos = m_GameObject->GetPosition();
	Vector3 playerpos = player->GetPosition();

	//ダメージリアクションが終わったら状態遷移
	if (m_Count > m_Recover && (pos - playerpos).Length() < 5.0)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
	}
	else if (m_Count > m_Recover)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
}

void EStateDamage::Draw()
{
	ImGui::Begin("EstateDamege");
	ImGui::SliderInt("Recover", &m_Recover, 20, 42);
	ImGui::End();
}
