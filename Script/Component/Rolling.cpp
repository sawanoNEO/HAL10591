#include "Rolling.h"
#include "../System/input.h"
#include "../System/manager.h"
#include "../Scene/scene.h"

#include "../GameObject/player.h"
#include "../Component/Rigidbody.h"
#include "../GameObject/camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Rolling::Init()
{
	startup = 2;
	invincible = 17;
	recovery = 15;
	m_Reception = 20;
	cnt = 0;
	m_Receptioncnt = 0;
}

void Rolling::Update()
{
	//�����ɕK�v�ȃI�u�W�F�N�g��R���|�[�l���g���擾����
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	PLAYERSTATE pstate=NONE;   ///�v���C���[�̏��
	Vector3 forward;
	Rigidbody* rb;
	Camera* camera;
	Vector3 camforward;//�J�����̑O�����x�N�g��
	Vector3 camside;   //�J�����̉������x�N�g��

	if (player)
	{
		pstate = player->GetPstate();
		forward = player->GetForward();
		rb = player->GetComponent<Rigidbody>();
		camera = scene->GetGameObject<Camera>();
		camforward = camera->GetCamForward();
		camside = camera->GetCamSide();

		if (pstate == DASH)
		{
			m_Receptioncnt++;
		}
		
		if (pstate == NONE &&     //�v���C���[������ȏ�ԂłȂ�
			Input::GetController(Input::a, Input::RELEASED) &&          //����{�^���𗣂�
			m_Receptioncnt < m_Reception&&                                  //�����t���ԓ��ł���
			player->GetST()>10.0)    /////���(�_�b�V��)�{�^���������Ă���m_Reception�Őݒ肳��Ă���t���[���܂ł͉�����󂯕t����
		{
			player->STUse(16.0);     //�X�^�~�i�������
			cnt++;
			pstate = ROLLING;//��Ԃ����[�����O�ɂ���
		}
		else if (pstate==ROLLING && cnt < startup)                  //���[�����O(�o�b�N�X�e�b�v)�̑O��
		{
			cnt++;
			if (!Input::GetStickState())              //�X�e�B�b�N�ɂ����͂����Ă��Ȃ�������o�b�N�X�e�b�v�ɂȂ�
			{
				pstate = BACKSTEP;
			}
			else
			{
				pstate = ROLLING;
			}

		}
		else if ((pstate == ROLLING ||pstate==BACKSTEP)&& cnt < startup + invincible)      //���G����
		{
			if (cnt == startup)              //���G���Ԃɓ������ŏ��̃t���[�������傫�ȗ͂�������
			{
				if (pstate==BACKSTEP)              //�X�e�B�b�N�ɂ����͂����Ă��Ȃ�������o�b�N�X�e�b�v�ɂȂ�
				{
					Vector3 vec =-forward * 100;
					rb->AddForce(vec, ForceMode::Impulse);              //�v���C���[�Ɉړ��̗͂�^����
					//rb->AddForce(-forward*100, ForceMode::Impulse);
				}
				else
				{
					Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//���[�����O�̕���������
					rb->AddForce(Rolvec*150, ForceMode::Impulse);              //�v���C���[�Ɉړ��̗͂�^����
					//rb->AddForce(forward*100, ForceMode::Impulse);
				}
			}
			else if (cnt > startup)
			{
				if (pstate == BACKSTEP) //�ȍ~�͎ア�͂�����������
				{
					rb->AddForce(-200 * forward, ForceMode::Acceleration);
				}
				else
				{
					rb->AddForce(Rolvec*150, ForceMode::Acceleration);
				}
			}
			cnt++;
		}
		else if ((pstate == ROLLING ||pstate==BACKSTEP)&& cnt < startup + invincible + recovery)//���[�����O�̌㌄
		{
			cnt++;
			////////////�㌄���̈�莞�Ԃ͍ēx����A�U�����\
			if ((pstate == ROLLING || pstate == BACKSTEP) && cnt > startup + invincible+7)
			{
				//��s���͂���������2023/11/10
				if (Input::GetController(Input::a, Input::RELEASED, 20) > 0 && m_Receptioncnt < m_Reception && player->GetST()>10.0)
				{
					cnt = 1;
					player->STUse(16.0);
				}
				else if (Input::GetController(Input::a, Input::UP))
				{
					m_Receptioncnt = 0;
				}
				else if (Input::GetController(Input::a, Input::HELD))
				{
					m_Receptioncnt++;
				}
				//��s���͎����O��r�p
				//if (Input::GetController(Input::a, Input::UP))
				//{
				//	m_Receptioncnt = 0;
				//}
				//else if (Input::GetController(Input::a, Input::HELD))
				//{
				//	m_Receptioncnt++;
				//}
				//else if (Input::GetController(Input::a, Input::RELEASED) && m_Receptioncnt < m_Reception&& player->GetST()>10.0)
				//{
				//	cnt = 1;
				//	player->STUse(16.0);
				//}
			}
			else
			{
				//�㌄�̏I���O�ɃX�e�B�b�N�ɂ����͂����Ă���΂�����x�ړ��ʂ��c��(����̓x�ɒ�~����ƋC�����悭�Ȃ�)
				rb->AddForce(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))) * 150, ForceMode::Acceleration);
				player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));
			}
		}
		else if ((pstate == ROLLING||pstate==BACKSTEP) && cnt >= startup + invincible + recovery)//�㌄���I�����A�e�ϐ��̃��Z�b�g���s��
		{
			pstate = NONE;
			cnt = 0;
			m_Receptioncnt = 0;
            //�㌄�I�����ɃX�e�B�b�N�ɂ����͂����Ă����Ȃ炠����x�ړ��ʂ�ۑ�����
			rb->AddForce(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)))*150, ForceMode::Acceleration);
			player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));

		}

		if(pstate==NONE&&Input::GetController(Input::a,Input::UP))
		{
			m_Receptioncnt = 0;
		}
		else if (Input::GetController(Input::a, Input::HELD))
		{
			m_Receptioncnt++;
		}
		player->SetPstate(pstate);
	}
}
