#include "StateDamage.h"
#include "StateNone.h"
#include "StateRolling.h"
#include "StateAttack.h"

#include "../GameObject/Player.h"

#include "../ImGui/imguimanager.h"

#include "../Scene/scene.h"
#include "../System/manager.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

void StateDamage::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->SetAnimSpeed(1.5f);

	if (m_Hitting == false)
	{
		m_GameObject->SetAnimName2("Impact");
		m_Count = 0;
	}
	m_Hitting = true;
}

void StateDamage::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->SetAnimSpeed(1.0f);

	if (m_Count > m_Recover)
	{
		m_Hitting = false;
	}
	player->SetInvincibleFrame(60);
}

void StateDamage::StateUpdate()
{
	m_Count++;
}

void StateDamage::StateChange()
{
	if (m_Count > m_Recover)
	{
	    if (Input::GetController(Input::R1, Input::PRESSED, 5))
	    {
	    	m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
	    }
	    else if (Input::GetController(Input::a, Input::RELEASED, 5))
	    {
	    	m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	    }
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateDamage::Draw()
{
#if _DEBUG
	ImGui::Begin("EstateDamege");
	ImGui::SliderInt("Recover", &m_Recover, 20, 42);
	ImGui::End();
#endif
}