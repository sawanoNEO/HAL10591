#include "StateMove.h"
#include "StateDash.h"
#include "StateRolling.h"
#include "StateAttack.h"
#include "StateNone.h"
#include "StateItem.h"

#include "../System/input.h"
#include "../GameObject/player.h"
#include "../Scene/scene.h"
#include "../ImGui/imguimanager.h"
#include "../System/manager.h"
#include "../GameObject/camera.h"
#include "../Component/Rigidbody.h"
#include "../Component/animationModel.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void StateMove::Enter()
{
	m_GameObject->SetAnimName2("Run");
	m_Accel = 70.0;//�����̒l�������l�ɖ߂�
	m_DAccel = 150.0;
	m_GameObject->PlaySE("Walk", true);
}

void StateMove::Exit()
{
	m_receptionCount = 0;
	m_GameObject->StopSE("Walk");
}

void StateMove::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	AnimationModel* anim = player->GetComponent<AnimationModel>();

	player->SetoldPosition(player->GetPosition());
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	Vector3 f = player->GetForward();   //�O�����x�N�g���擾
	Vector3 fNormalR = player->GetSide();
	Vector3 fNormalL = -player->GetSide();
	Vector3 s = player->GetSide();
	Vector3 pos = player->GetPosition();
	Vector3 currentRot = player->GetRotation();//��]�擾
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	float ST = player->GetST();
	bool Wait = player->GetWait();

	player->STRecover();

	player->SetRotation(currentRot);
	//�����܂�


	if (Input::GetStickState()==true)        //�X�e�B�b�N�ɂ�鑀�삪�s���Ă��Ȃ��Ɖ����x�����Z�b�g����
	{
		//pos += (XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * Accel);

		//�ϐ�����vec�������ۂ͂ǂ̕����ɂǂ̂��炢�̗͂������邩���v�Z���A�i�[���Ă���
		Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * m_Accel;
		rb->AddForce(vec, ForceMode::Acceleration);              ///�����x�����Ƀv���C���[�Ɉړ��̗͂�^����
		player->SetPosition(pos);

		//�v���C���[������������ݒ肷��
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX))));

		m_Accel += m_Accel * 0.1;//�����x�̉��Z
		m_DAccel = 150.0;
	}
	if (m_Accel >= 150)  //���̗ʂ𒴂��Ȃ��悤�ɂ���
	{
		m_Accel = 150;
	}
	if (m_DAccel >= 250)
	{
		m_DAccel = 250;
	}

	if (Input::GetController(Input::R1, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
	}

	//if (camera->GetRock())//�^�[�Q�b�g�J������Ԃ̎��̏���
	//{
	//	Vector3 currentRot = player->GetRotation();
	//	Vector3 enemypos = camera->GetRockEnemy()->GetPosition();
	//	float fx = enemypos.x - pos.x;
	//	float fz = enemypos.z - pos.z;
	//	Vector3 nom = { fx,0.0,fz };
	//	nom.Normalize();
	//	float afo = atan2(nom.x, nom.z);
	//	currentRot.y = afo;
	//	player->SetRotation(currentRot);
	//	player->SetpromissDirection(f);
	//}
}

void StateMove::Draw()
{
}

void StateMove::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	float ST = player->GetST();
	float Wait = player->GetWait();

	if (Input::GetStickState() == false)
	{
		//��Ԃ𖳑����ԂɕύX
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
	else if (Input::GetController(Input::a, Input::HELD) &&
		Input::GetStickState() && ST > 0 && !Wait)        //�X�e�B�b�N�ɂ�鑀�삪�s���Ă��Ȃ��Ɖ����x�����Z�b�g����
	{
		m_receptionCount++;
		if (m_receptionCount > m_reception)//�P�\�ȏ�A�{�^����������Ă���΃_�b�V����ԂɈڍs
		{
			//��Ԃ��_�b�V���ɕύX
			m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateDash>());
		}
	}
	else if (Input::GetController(Input::a, Input::RELEASED) && ST > 0 && !Wait)
	{
		//��Ԃ�����ɕύX
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	}
	else if (Input::GetController(Input::x, Input::PRESSED))
	{
		//��Ԃ��A�C�e���g�p�ɕύX
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateItem>());
	}
}
