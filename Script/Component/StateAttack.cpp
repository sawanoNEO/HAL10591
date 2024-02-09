#include "StateAttack.h"
#include "StateNone.h"
#include "Colider.h"
#include "Rigidbody.h"
#include "audio.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/player.h"
#include "../GameObject/enemy.h"
#include "../GameObject/Boss.h"
#include "../GameObject/camera.h"
#include "../GameObject/Effect/Slash.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void StateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	AttackObj = scene->AddGameObject<GameObject>(1);
	AttackObj->AddComponent<Colider>();
	AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	m_Maxcombo = 2;
	m_combo = 0;
	m_Startup = 22;
	m_ActiveFrames = 5;
	m_Recovery = 23;
	m_Count = 0;
	m_Power = 45;
	m_Staminaconsumption = 17.0;
}

void StateAttack::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimName2("Attack");
	m_Count = 0;
	player->SetAnimSpeed(2.0f);
	//player->SetFrame1();
}

void StateAttack::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimSpeed(1.0f);
}

void StateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = player->GetForward();
	std::vector<Enemy*> enemys = scene->GetGameObjects<Enemy>();
	std::vector<Colider*> col;//�I�u�W�F�N�g�̓����蔻����擾����
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	bool hit = false;
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	std::unordered_map<const char*,Audio*> se = player->GetSEs();

	AttackObj->SetPosition(player->GetPosition()+forward*3);

	////�G�̃R���C�_�[���擾
	enemys = Enemy::GetEnemys();
	col.resize(enemys.size());
	for (int i=0;i<enemys.size();i++)
	{
		col[i] = enemys[i]->GetComponent<Colider>();
	}
	//col.resize(enemys.size()+Bosses.size());
	//for (int i = 0; i < enemys.size(); i++)
	//{
	//	col[i] = enemys[i]->GetComponent<Colider>();
	//}
	//for (int i = 0; i < Bosses.size(); i++)
	//{
	//	col[i + enemys.size()] = Bosses[i]->GetComponent<Colider>();
	//}

	if (m_Count < m_Startup)//�U���̗\������
	{

		m_Staminaconsumption = 17.0f;

		//���Ƀq�b�g�����Ƃ�����������Z�b�g
		for (const auto enemy : enemys)
		{
			enemy->HitReset();
		}

	}
	else if (m_Count < m_Startup +m_ActiveFrames)/////�U�����肪�o�Ă��鎞�ԁB���������B
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));

		//�U������������u�Ԃ����s������
		if (m_Count == m_Startup)
		{
			//�G�t�F�N�g�ƂȂ�I�u�W�F�N�g��O���ɐ���
			slash = scene->AddGameObject<Slash>(1);
			slash->SetColor(White);
			Vector3 pos = player->GetPosition() + player->GetForward() * 6;
			pos.y += 2.0;
			//slash->SetPosition(player->GetPosition() + player->GetForward() * 5);
			slash->SetPosition(pos);
			Vector3 rot = { 0.0,0.0,0.0 };
			rot.z += cos(90 * 3.14 / 180);
			slash->SetRotation(rot);

			player->STUse(m_Staminaconsumption);
			Vector3 vec = player->GetForward() * 100.0f;
			if (Input::GetStickState())
			{
				Vector3 vec = player->GetpromissDirection() * 100.0f;
			}
			rb->AddForce(vec, ForceMode::Impulse);

			//�������؂鉹���Đ�
			m_SENumber = rand() % 3;//�Đ����鉹������
			switch (m_SENumber)
			{
			case 0:
				se["Swing1"]->Play(false);
				break;
			case 1:
				se["Swing2"]->Play(false);
				break;
			case 2:
				se["Swing3"]->Play(false);
				break;
			default:
				break;
			}
		}

		if (enemys.size() == 0)
		{
			return;
		}

		//�G�ƍU�����肪�ڐG���Ă��邩���肷��
		for (int i = 0; i < enemys.size(); i++)
		{
			Colider* hitobj = col[i]->CollisionAABB(AttackObj->GetComponent<Colider>()->GetAABB(), col[i]);

			if (hitobj != nullptr && hitobj->GetTug() == ENEMY)
			{
			    //�U�����q�b�g�������̏���
				if (enemys[i] && !hit)
				{
					switch (m_SENumber)
					{
					case 0:
						se["Swing1"]->Stop();
						se["Puntch1"]->Play(false);
						break;
					case 1:
						se["Swing2"]->Stop();
						se["Puntch2"]->Play(false);
						break;
					case 2:
						se["Swing3"]->Stop();
						se["Puntch3"]->Play(false);
						break;
					default:
						break;
					}
					Vector3 enemypos = enemys[i]->GetPosition();
					enemypos.y += 1.0f;
					enemys[i]->Damage(m_Power);
					slash->SetColor(Red);
					//slash->SetPosition(enemypos);
				}
			}
		}
	}
	else if (m_Count < m_Startup +m_ActiveFrames + m_Recovery)         //�U����̍d�����ԁB���̊Ԃ̈��̃^�C�~���O�ł��������ǍU���{�^���������ƘA���ɂȂ�
	{
		if (Input::GetController(Input::R1, Input::PRESSED,15)!=-1 &&
			player->GetST() > 0.0f &&
			m_Count > m_Startup +m_ActiveFrames + (m_Recovery / 3))     //�d�����n�܂��Ă����͘A���Ɉڍs�ł��Ȃ�
		{
			m_Count = m_Startup - 10;
			player->SetFrame1(0);
			//player->STUse(17.0f);
		}
	}
	
	m_Count++;
}

void StateAttack::StateChange()
{
	if (m_Count > m_Startup +m_ActiveFrames + m_Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateAttack::Draw()
{
#if _DEBUG
	ImGui::Begin("StateAttack");
	ImGui::SliderInt("Recovery", &m_Recovery, -20, 20);
	ImGui::End();
#endif
}
