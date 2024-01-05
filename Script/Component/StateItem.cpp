#include "StateItem.h"
#include "Item.h"
#include "Heal.h"
#include "StateNone.h"
#include "../GameObject/gameObject.h"

void StateItem::Init(ITEMID _id)
{
	Item* item;
	switch (_id)
	{
	case HEAL:
		item = new Heal;
		item->Init();
		m_Items.push_back(item);
		m_Items[0]->SetGameObject(m_GameObject);
		m_Cursol = 0;
		break;
	default:
		break;
	}
}

void StateItem::Uninit()
{
}

void StateItem::Enter()
{
	m_GameObject->SetAnimName2(m_Items[m_Cursol]->GetAnimName());
	m_Items[m_Cursol]->Enter();
}

void StateItem::Exit()
{
	m_Items[m_Cursol]->Exit();
}

void StateItem::StateUpdate()
{
	m_Items[m_Cursol]->Update();
}

void StateItem::StateChange()
{
	if (m_Items[m_Cursol]->GetChangeFlg()==true)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateItem::Draw()
{
}
