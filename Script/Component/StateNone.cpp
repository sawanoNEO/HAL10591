#include "StateNone.h"

#include "../GameObject/player.h"

#include "../Scene/scene.h"

#include "../System/manager.h"
#include "../System/input.h"

#include "StateMachine.h"
#include "StateMove.h"


void StateNone::Enter()
{
	m_GameObject->SetAnimName2("Idle");
}

void StateNone::Exit()
{
}

void StateNone::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	

	if (Input::GetStickState())
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateMove>());
	}
}

void StateNone::Draw()
{
}
