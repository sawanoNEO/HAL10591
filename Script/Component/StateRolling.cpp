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

void StateRolling::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->STUse(16.0);     //�X�^�~�i�������
	player->SetAnimSpeed(2.0f);
	m_GameObject->SetAnimName2("Rolling");
	cnt = 0;
}

void StateRolling::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->SetAnimSpeed(1.0f);//�Đ����x�����ɖ߂�
	cnt = 0;
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

	forward = player->GetForward();
	rb = player->GetComponent<Rigidbody>();
	camera = scene->GetGameObject<Camera>();
	camforward = camera->GetCamForward();
	camside = camera->GetCamSide();

	if (cnt < startup)
	{
		if (Input::GetStickState())
		{
			Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//���[�����O�̕���������
		}
		else
		{
			Rolvec = player->GetForward();
		}
		player->SetpromissDirection(Rolvec);
		rb->AddForce(Rolvec * 75, ForceMode::Impulse);              //�v���C���[�Ɉړ��̗͂�^����
	}
	else if (cnt < startup + invincible)
	{
		player->SetInvincible(true);
		rb->AddForce(Rolvec * 150, ForceMode::Acceleration);              //�v���C���[�Ɉړ��̗͂�^����
	}
	else if (cnt <= startup + invincible + recovery)
	{
		player->SetInvincible(false);
		if (Input::GetController(Input::a, Input::RELEASED, 20) != -1)
		{
			this->Enter();
		}
	}
		cnt++;
}

void StateRolling::StateChange()
{
	if (cnt > startup+invincible+recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateRolling::Draw()
{
}
