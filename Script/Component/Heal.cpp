#include "Heal.h"
#include "Rigidbody.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/player.h"

using namespace DirectX::SimpleMath;

void Heal::Enter()
{
	m_MaxInventoryCount = 10;
	m_InventoryCount = 3;
	m_StartUpTime = 30;
	m_ActionTime = 1;
	m_RecoveryTime = 70;
	AnimName = "Drink";
	m_Count = 0;
	m_ChangeStateFlg = false;
}

void Heal::Exit()
{
}

void Heal::Init()
{
	m_MaxInventoryCount = 10;
	m_InventoryCount = 3;
	m_StartUpTime = 30;
	m_ActionTime = 1;
	m_RecoveryTime = 70;
	AnimName = "Drink";
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
	
	if (m_Count < m_StartUpTime)
	{
	}
	else if (m_Count < m_StartUpTime + m_ActionTime)
	{
		player->ASHP(650);
	}
	else if (m_Count < m_StartUpTime + m_ActionTime + m_RecoveryTime)
	{
		if (Input::GetController(Input::x, Input::PRESSED))
		{
			m_Count = 15;
			m_GameObject->SetFrame1(15);
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
}
