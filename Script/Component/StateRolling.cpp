#include "StateRolling.h"
#include "StateNone.h"
#include "Rigidbody.h"

#include "../System/input.h"
#include "../System/manager.h"
#include "../Scene/scene.h"

#include "../GameObject/player.h"
#include "../GameObject/camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void StateRolling::
Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->STUse(16.0);     //�X�^�~�i�������
	player->SetAnimSpeed(2.0f);
	m_GameObject->SetAnimName2("Rolling");
	m_Count = 0;
	if (Input::GetController(Input::a, Input::HELD))
	{
		m_KeepABurron = true;
	}
}

void StateRolling::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->SetAnimSpeed(1.0f);//�Đ����x�����ɖ߂�
	m_Count = 0;
	m_KeepABurron = false;
}

void StateRolling::StateUpdate()
{
	//�����ɕK�v�ȃI�u�W�F�N�g��R���|�[�l���g���擾����
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward;
	Rigidbody* rb;
	Camera* camera;
	Vector3 camforward;//�J�����̑O�����x�N�g��
	Vector3 camside;   //�J�����̉������x�N�g��
	float Sutamina = player->GetST();
	forward = player->GetForward();
	rb = player->GetComponent<Rigidbody>();
	camera = scene->GetGameObject<Camera>();
	camforward = camera->GetCamForward();
	camside = camera->GetCamSide();

	if (m_Count < m_Startup)
	{
		if (Input::GetStickState())
		{
			m_Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//���[�����O�̕���������
		}
		else
		{
			m_Rolvec = player->GetForward();
		}
		player->SetpromissDirection(m_Rolvec);
		m_GameObject->PlaySE("Rolling");
		rb->AddForce(m_Rolvec * 60, ForceMode::Impulse);              //�v���C���[�Ɉړ��̗͂�^����
	}
	else if (m_Count < m_Startup + m_Invincible)
	{
		player->SetInvincible(true);
		rb->AddForce(m_Rolvec * 120, ForceMode::Acceleration);              //�v���C���[�Ɉړ��̗͂�^����
	}
	else if (m_Count <= m_Startup + m_Invincible + m_Recovery)
	{
		player->SetInvincible(false);
		if (Input::GetController(Input::a, Input::RELEASED, 10) != -1&&      //10f�O�܂ł̒��ŁAA�{�^���������ꂽ�u�Ԃ��Ȃ����-1��Ԃ�
			m_KeepABurron==false&&Sutamina>5.0)
		{
			player->SetFrame1(0.0f);
			this->Enter();
		}
	}
		m_Count++;
}

void StateRolling::StateChange()
{
	if (m_Count > m_Startup+m_Invincible + m_Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateRolling::Draw()
{
}
