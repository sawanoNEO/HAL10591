#include "StateNone.h"
#include "StateMove.h"
#include "StateRolling.h"
#include "StateAttack.h"
#include "StateItem.h"

#include "../GameObject/player.h"

#include "../Scene/scene.h"

#include "../System/manager.h"
#include "../System/input.h"


void StateNone::Enter()
{
	m_GameObject->SetAnimName2("Idle");
	m_ReceptionCount = 0;
	if (Input::GetController(Input::a, Input::HELD))
	{
		m_KeepAButton = true;
	}
}

void StateNone::Exit()
{
	m_KeepAButton = false;
}

void StateNone::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	
	player->STRecover();//�X�^�~�i�̉񕜂��s��

	//////�e�s���̌��A�{�^�����������ςȂ��ɂ��Ă��Ă�����Ȃǂ��\�����Ȃ��悤�ɂ���
	if (m_KeepAButton == true)
	{
		m_ReceptionCount = 0;
	}
	if (Input::GetController(Input::a, Input::UP))
	{
		m_KeepAButton = false;
	}
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
		m_ReceptionCount++;
	}
	//���
	if (m_ReceptionCount > m_Reception || (Input::GetController(Input::a, Input::RELEASED)&&m_KeepAButton!=true))//��莞��A��������A�𗣂����Ƃŉ���ɔh��
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	}
	//�U��
	if (Input::GetController(Input::R1, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
	}
	//�A�C�e�����g��
	if (Input::GetController(Input::x, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateItem>());
	}
}

void StateNone::Draw()
{
}
