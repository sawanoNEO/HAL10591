#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "enemy.h"
#include "player.h"
#include "EnemyHP.h"

#include "../Component/Rigidbody.h"
#include "../Component/shader.h"
#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../Component/AnimationManager.h"
#include "../Component/animationModel.h"
#include "../Component/EnemyState/EStateNone.h"
#include "../Component/EnemyState/EStateDamage.h"
#include "../Component/EnemyState/EStateChase.h"
#include "../Component/EnemyState/EStateAttack.h"
#include "../Component/EnemyState/EStateWaitAndSee.h"
#include "../Component/StateDeath.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Enemy::Init()
{
	MaxHP = 100.0;
	HP = MaxHP;
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset\\model\\Player\\Paladin J Nordstrom.fbx");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\Player\\Impact.fbx", "Impact");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash.fbx", "Attack");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash3.fbx", "Attack3");
	m_Model->LoadAnimation("asset\\model\\Player\\EnemyRightStrafe.fbx", "EnemyRightStrafe");
	m_Model->LoadAnimation("asset\\model\\Player\\EnemyLeftStrafe.fbx", "EnemyLeftStrafe");
	m_Model->LoadAnimation("asset\\model\\Player\\Death.fbx", "Death");

	AddComponent<EStateNone>();
	AddComponent<EStateDamage>();
	AddComponent<EStateChase>();
	AddComponent<EStateAttack>();
	AddComponent<EStateWaitandSee>();
	AddComponent<StateDeath>();
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<EStateNone>());

	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	AddComponent<Shadow>()->SetSize(2.0f);
	AddComponent<Colider>()->Init(ENEMY, Vector3(100.0f,100.0f,100.0f));
	rb = AddComponent<Rigidbody>();
	rb->Init(5.0f);

    m_HP = AddChild<EnemyHP>();

}

void Enemy::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Colider* Pcol;
	Vector3 forward;
	Vector3 playerpos;
	const char* Animname1 = m_Animname1.c_str();//�A�j���[�V�����̖��O1
	const char* Animname2 = m_Animname2.c_str();//�A�j���[�V�����̖��O2

	m_HP->deliverParamater(HP);
	m_HP->SetPosition(Vector3(m_Position.x, m_Position.y+2.0f, m_Position.z));


	if (!player)
	{
		GetComponent<StateMachine>()->changeState(GetComponent<EStateNone>());
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

void Enemy::Draw()
{
	ImGui::Begin("Enemy");
	ImGui::Text("HP=%f\n\n", HP);
	ImGui::Text("m_Frame1=%f", m_Frame1);
	ImGui::Text("m_Frame2=%f", m_Frame2);
	ImGui::Text("POS...%f,%f,%f\n", m_Position.x, m_Position.y, m_Position.z);
	ImGui::SliderFloat("HP", &HP, 0, 100);
	ImGui::Text("HPPos%f%f%f", m_HP->GetPosition().x, m_HP->GetPosition().y, m_HP->GetPosition().z);
	ImGui::End();
}

void Enemy::Damage(float damage)
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

void Enemy::HitReset()
{
	hit = false;
}



void Enemy::SetAnimName2(const char* _Name)
{
	assert(m_Model->CheckAnimData(_Name) != false && "�w��̃A�j���[�V������������܂���ł����B�����̖��O��A�f�[�^�������Ă��邩�m�F���Ă��������B");

	//�Đ��A�j���[�V�����̕ύX�A�e�A�j���[�V�����֘A�̕ϐ��̃��Z�b�g
	m_Animname1 = m_Animname2;
	m_Animname2 = _Name;
	m_Frame1 = m_Frame2;
	m_Frame2 = 0;
	m_BlendRate = 0.0;
}