#include "Heal.h"
#include "Rigidbody.h"

#include "../Scene/scene.h"

#include "../System/manager.h"
#include "../System/renderer.h"

#include "../GameObject/player.h"
#include "../GameObject/Effect/HealEffect.h"
#include "../GameObject/score.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void Heal::Enter()
{
	m_StartUpTime = 30;
	m_ActionTime = 1;
	m_RecoveryTime = 70;
	m_AnimName = "Drink";
	m_Count = 0;
	m_ChangeStateFlg = false;
}


void Heal::Exit()
{
}

void Heal::Init()
{
	m_MaxInventoryCount = 5;
	m_InventoryCount = m_MaxInventoryCount;
	m_StartUpTime = 30;
	m_ActionTime = 1;
	m_RecoveryTime = 70;
	m_AnimName = "Drink";
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	score->AddCount(m_MaxInventoryCount);
}

void Heal::Uninit()
{
}

void Heal::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 currentRot = player->GetRotation();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	Score* score = scene->GetGameObject<Score>();
	
	player->STRecover();//スタミナの自動回復を行う

	if (m_Count < m_StartUpTime)
	{
	}
	else if (m_Count == m_StartUpTime)
	{
		//アイテムの所持数をチェック
		if (m_InventoryCount > 0)
		{
			m_GameObject->PlaySE("Heal");
		}
		else
		{
			m_GameObject->PlaySE("Miss");
		}
	}
	else if (m_Count == m_StartUpTime + m_ActionTime)
	{
		//アイテムの所持数をチェック
		if (m_InventoryCount > 0)
		{
			Score* score = scene->GetGameObject<Score>();
			m_InventoryCount--;
			score->AddCount(-1);
			player->ASHP(650);

			//パーティクルによるエフェクト表示
			for (int i = 0; i < m_ParticleNum; i++)
			{
				HealEffect* effect = scene->AddGameObject<HealEffect>(1);
				Vector3 effectpos = m_GameObject->GetPosition();
				effectpos.x += (rand() % 40 - 20) * 0.1;
				effectpos.y += 0.5 + (rand() % 20) * 0.1;
				effectpos.z += (rand() % 40 - 20) * 0.1;
				effect->SetPosition(effectpos);
			}
		}
	}
	else if (m_Count < m_StartUpTime + m_ActionTime + m_RecoveryTime)
	{
		if (m_InventoryCount > 0)
		{
			if (Input::GetController(Input::x, Input::PRESSED,10)!=-1)//回復の連続使用
			{
				m_Count = 15;
				m_GameObject->SetFrame1(15);
			}

			//パーティクルによるエフェクト表示
			for (int i = 0; i < m_ParticleNum; i++)
			{
				HealEffect* effect = scene->AddGameObject<HealEffect>(1);
				Vector3 effectpos = m_GameObject->GetPosition();
				effectpos.x += (rand() % 40 - 20) * 0.1;
				effectpos.y += 0.5 + (rand() % 20) * 0.1;
				effectpos.z += (rand() % 40 - 20) * 0.1;
				effect->SetPosition(effectpos);
			}
		}
	}
	else if (m_Count >= m_StartUpTime + m_ActionTime + m_RecoveryTime)
	{
		m_ChangeStateFlg = true;
	}

	m_Count++;
}

void Heal::Draw()
{
#if _DEBUG
	ImGui::Begin("Heal");
	ImGui::SliderInt("ParticleNum", &m_ParticleNum, 0, 20);
	ImGui::End();
#endif
}

