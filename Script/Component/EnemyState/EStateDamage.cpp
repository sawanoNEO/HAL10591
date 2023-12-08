#include "EStateDamage.h"
#include "EStateNone.h"

#include "../../GameObject/gameObject.h"

void EStateDamage::Enter()
{
	m_GameObject->SetAnimName2("Impact");
	m_Count = 0;
}

void EStateDamage::Exit()
{
}

void EStateDamage::StateUpdate()
{
	if (m_Count > 42)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
	m_Count++;
}

void EStateDamage::Draw()
{
}
