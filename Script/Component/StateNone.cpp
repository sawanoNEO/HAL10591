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
	
	player->STRecover();//�X�^�~�i�̉񕜂��s��
}

void StateNone::StateChange()
{
	//�ړ�
	if (Input::GetStickState())
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateMove>());
	}

	if (Input::GetController(Input::a, Input::PRESSED) || Input::GetController(Input::a, Input::HELD))
	{
		receptionCount++;
	}
	//���
	if (receptionCount > reception || Input::GetController(Input::a, Input::RELEASED))//��莞��A��������A�𗣂����Ƃŉ���ɔh��
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	}
	//�U��
	if (Input::GetController(Input::R1, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
	}
}

void StateNone::Draw()
{
}
