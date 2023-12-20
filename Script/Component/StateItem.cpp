#include "StateItem.h"
#include "Item.h"
#include "Heal.h"
#include "StateNone.h"
#include "../GameObject/gameObject.h"

void StateItem::Init(Item* _item)
{
	m_CurrentItem = _item;
}

void StateItem::Uninit()
{
}

void StateItem::Enter()
{
	m_GameObject->SetAnimName2(m_CurrentItem->GetAnimName());
}

void StateItem::Exit()
{
}

void StateItem::StateUpdate()
{
	m_CurrentItem->Update();
}

void StateItem::StateChange()
{
	if (m_CurrentItem->GetChangeFlg())
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateItem::Draw()
{
}
