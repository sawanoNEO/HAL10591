#include "StateDeath.h"

#include "../GameObject/gameObject.h"

void StateDeath::Enter()
{
	m_GameObject->SetAnimName2("Death");
	m_GameObject->SetAnimSpeed(2.0f);
	m_Count = 0;
}

void StateDeath::Exit()
{
}

void StateDeath::StateUpdate()
{
	if (m_Count > m_DeathFrame)
	{
		m_GameObject->SetDestroy();
	}
	m_Count++;
}

void StateDeath::StateChange()
{
}

void StateDeath::Draw()
{
}
