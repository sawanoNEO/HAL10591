#include "StateNone.h"
#include "StateMove.h"
#include "StateRolling.h"
#include "StateAttack.h"

#include "../GameObject/player.h"

#include "../Scene/scene.h"

#include "../System/manager.h"
#include "../System/input.h"


void StateNone::Enter()
{
	m_GameObject->SetAnimName2("Idle");
	receptionCount = 0;
}

void StateNone::Exit()
{
}

void StateNone::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	
	player->STRecover();//スタミナの回復を行う
}

void StateNone::StateChange()
{
	//移動
	if (Input::GetStickState())
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateMove>());
	}

	if (Input::GetController(Input::a, Input::PRESSED) || Input::GetController(Input::a, Input::HELD))
	{
		receptionCount++;
	}
	//回避
	if (receptionCount > reception || Input::GetController(Input::a, Input::RELEASED))//一定時間A長押しかAを離すことで回避に派生
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	}
	//攻撃
	if (Input::GetController(Input::R1, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
	}
}

void StateNone::Draw()
{
}
