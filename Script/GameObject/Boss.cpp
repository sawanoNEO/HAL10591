#include "Boss.h"
#include "EnemyHP.h"
#include "player.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../Component/AnimationManager.h"
#include "../Component/animationModel.h"
#include "../Component/shader.h"
#include "../Component/EnemyState/EStateNone.h"
#include "../Component/EnemyState/EStateDamage.h"
#include "../Component/EnemyState/EStateChase.h"
#include "../Component/EnemyState/EStateAttack.h"
#include "../Component/EnemyState/EStateWaitAndSee.h"
#include "../Component/StateDeath.h"

#include "../Component/audio.h"
#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../Component/Rigidbody.h"

#include <fstream>
#include <SimpleMath.h>
#include <iostream>

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace std;

void Boss::Init()
{
	m_FileDataPath = "asset\\editer\\BossData.csv";
	m_Number = m_Enemyes.size();
	m_Enemyes.push_back(this);

	//////ifstream BossData("asset\\editer\\BossData.csv");
	//////string test;

	//////assert(BossData.is_open()&&"�t�@�C�����J���܂���ł���");

	//////getline(BossData, test);
	//////getline(BossData, test);
	//////BossData.close();
	MaxHP = 1000.0;
	HP = MaxHP;
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset\\model\\Boss\\Boss.fbx");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossRun.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossDamage.fbx", "Impact");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossSwiping.fbx", "Attack");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash3.fbx", "Attack3");
	m_Model->LoadAnimation("asset\\model\\Player\\EnemyRightStrafe.fbx", "EnemyRightStrafe");
	m_Model->LoadAnimation("asset\\model\\Player\\EnemyLeftStrafe.fbx", "EnemyLeftStrafe");
	m_Model->LoadAnimation("asset\\model\\PowerUp.fbx", "BossAppearance");
	SetAnimName2("BossAppearance");
	SetAnimName2("BossAppearance");
	//SetAnimSpeed(0.5f);
	AddComponent<EStateNone>();
	AddComponent<EStateDamage>();
	AddComponent<StateDeath>();
	AddComponent<EStateChase>()->Init(m_FileDataPath);
	//AddComponent<EStateAttack>()->Init(25, 7, 19, 350, 0, Vector3{6.0f,2.0f,4.0f});
	AddComponent<EStateAttack>()->Init("asset\\editer\\BossData.csv");
	AddComponent<EStateWaitandSee>();
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<EStateNone>());

	m_SE["Puntch1"] = AddComponent<Audio>();
	m_SE["Puntch1"]->Load("asset\\audio\\SE\\Puntch1.wav");
	m_SE["Puntch2"] = AddComponent<Audio>();
	m_SE["Puntch2"]->Load("asset\\audio\\SE\\Puntch2.wav");
	m_SE["Puntch3"] = AddComponent<Audio>();
	m_SE["Puntch3"]->Load("asset\\audio\\SE\\Puntch3.wav");
	m_SE["Swing1"] = AddComponent<Audio>();
	m_SE["Swing1"]->Load("asset\\audio\\SE\\Swing1.wav");
	m_SE["Swing2"] = AddComponent<Audio>();
	m_SE["Swing2"]->Load("asset\\audio\\SE\\Swing2.wav");
	m_SE["Swing3"] = AddComponent<Audio>();
	m_SE["Swing3"]->Load("asset\\audio\\SE\\Swing3.wav");
	m_SE["Walk"] = AddComponent<Audio>();
	m_SE["Walk"]->Load("asset\\audio\\SE\\���̏�1.wav");
	m_SE["Dash"] = AddComponent<Audio>();
	m_SE["Dash"]->Load("asset\\audio\\SE\\���̏�2.wav");


	m_Scale = Vector3(0.04f, 0.04f, 0.04f);

	AddComponent<Shadow>()->SetSize(2.0f);
	AddComponent<Colider>()->Init(ENEMY, Vector3(100.0f, 100.0f, 100.0f));
	rb = AddComponent<Rigidbody>();
	rb->Init(5.0f);

	//m_HP = AddChild<EnemyHP>();
}

void Boss::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Colider* Pcol;
	Vector3 forward;
	Vector3 playerpos;
	const char* Animname1 = m_Animname1.c_str();//�A�j���[�V�����̖��O1
	const char* Animname2 = m_Animname2.c_str();//�A�j���[�V�����̖��O2

	//HP�o�[�̕\���ʒu
	//m_HP->deliverParamater(HP);
	//m_HP->SetPosition(Vector3(m_Position.x, m_Position.y + 5.0f, m_Position.z));


	if (!player)
	{
		//GetComponent<StateMachine>()->changeState(GetComponent<EStateNone>());
	}

	//if (player)
	//{
	//	Pcol = player->GetComponent<Colider>();
	//	forward = XMVector3Normalize(player->GetPosition() - m_Position);
	//	playerpos = player->GetPosition();
	//}

	m_Model->Update(Animname1, m_Frame1, Animname2, m_Frame2, m_BlendRate);

	Vector3 vel = rb->GetVelocity();
	Vector3 acc = rb->GetAccel();
	Vector3 force = rb->GetForce();
	float mass = rb->GetMass();
	//�ʒu���O�ȉ��Ȃ�n�ʈʒu�ɃZ�b�g����
	if (m_Position.y < groundHeight && vel.y < 0.0f)
	{
		m_Position.y = groundHeight;       //���̍����Ŏ~�܂�
		vel.y = 0.0f;                      //�����鑬�x�����Z�b�g
		acc = force / mass;                //�����x�𖀎C�Ō���������
		rb->SetAccel(acc);
		rb->SetVelocity(vel);
	}

	//�G�ƐڐG���Ă����ꍇ�Ɉړ��x��0�ɂ��鏈��
	std::array<std::list<Colider*>, HITDIRMAX> coliders = GetComponent<Colider>()->GetAllHitColiders();
	for (int i = 0; i < HITDIRMAX; i++)
	{
		for (auto itr : coliders[i])
		{
			if (itr->GetTug() == PLAYER)
			{
				switch (i)
				{
				case TOP:
					m_Position.z = m_OldPosition.z;
					vel.z = 0.0;
					break;
				case BOTTOM:
					m_Position.z = m_OldPosition.z;
					vel.z = 0.0;
					break;
				case RIGHT:
					m_Position.x = m_OldPosition.x;
					vel.x = 0.0;
					break;
				case LEFT:
					m_Position.x = m_OldPosition.x;
					vel.x = 0.0;
					break;
				}
			}
		}
	}

	//�A�j���[�V�����̃t���[����i�߂�
	if (m_BlendRate < 1.0f)
	{
		m_BlendRate += 0.1f;
		m_Frame2 += 1.0f * m_AnimSpeed;
	}
	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0;
	}
	if (m_BlendRate == 1.0f)
	{
		m_Animname1 = m_Animname2;
	}
	m_Frame1 += 1.0f * m_AnimSpeed;
}

void Boss::Draw()
{
}

void Boss::Damage(float damage)
{
	if (!hit)
	{
		HP -= damage;
		if (HP <= 0)
		{
			GetComponent<StateMachine>()->changeState(GetComponent<StateDeath>());
		}
		else
		{
			GetComponent<StateMachine>()->changeState(GetComponent<EStateDamage>());
		}
		hit = true;
	}
}

void Boss::HitReset()
{
	hit = false;
}

void Boss::SetAnimName2(const char* _Name)
{
	assert(m_Model->CheckAnimData(_Name) != false && "�w��̃A�j���[�V������������܂���ł����B�����̖��O��A�f�[�^�������Ă��邩�m�F���Ă��������B");

	//�Đ��A�j���[�V�����̕ύX�A�e�A�j���[�V�����֘A�̕ϐ��̃��Z�b�g
	m_Animname1 = m_Animname2;
	m_Animname2 = _Name;
	m_Frame1 = m_Frame2;
	m_Frame2 = 0;
	m_BlendRate = 0.0;
}
