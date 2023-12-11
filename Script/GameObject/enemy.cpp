#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "enemy.h"
#include "player.h"

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
	
	AddComponent<EStateNone>();
	AddComponent<EStateDamage>();
	AddComponent<EStateChase>();
	AddComponent<EStateAttack>();
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<EStateNone>());

	m_Scale = Vector3(0.02f, 0.02f, 0.02f);

	AddComponent<Shadow>()->SetSize(2.0f);
	AddComponent<Colider>()->Init(ENEMY, Vector3(100.0f,100.0f,100.0f));
	rb = AddComponent<Rigidbody>();
	rb->Init(5.0f);
	state = NORMAL;

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

		//state = NONE;
	switch (state)
	{
	case Enemy::NONE:
		break;
	case Enemy::NORMAL:
		//if (SearchPlayer(playerpos,m_Position,30.0f,15.0f) == true)////�v���C���[������͈͂ɂ������Ԃ�J�ڂ���
		//{
		//	state = BATTLE;
		//}
		//break;
	case Enemy::BATTLE:
		/*rb->AddForce(forward*200.0, ForceMode::Force);
		if (player)
		{
			if (GetComponent<Colider>()->CollisionAABB(GetComponent<Colider>()->GetAABB(), Pcol).GetTug() == PLAYER && !Phit&&player->GetInvincible()==false)
			{
				player->ASHP(-200);
				Phit = true;
			}
			else if (GetComponent<Colider>()->CollisionAABB(GetComponent<Colider>()->GetAABB(), Pcol).GetTug() != PLAYER && Phit)
			{
				Phit = false;
			}
		}*/
		break;
	default:
		break;
	}
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

	//�A�j���[�V�����̃t���[����i�߂�
	if (m_BlendRate < 1.0f)
	{
		m_BlendRate += 0.1f;
		m_Frame2 += 1.0f;
	}
	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0;
	}
	if (m_BlendRate == 1.0f)
	{
		m_Animname1 = m_Animname2;
	}
	m_Frame1 += 1.0f;
}

void Enemy::Draw()
{
	ImGui::Begin("Enemy");
	ImGui::Text("HP=%f\n\n", HP);
	ImGui::Text("m_Frame1=%f", m_Frame1);
	ImGui::Text("m_Frame2=%f", m_Frame2);
	ImGui::Text("POS...%f,%f,%f\n", m_Position.x, m_Position.y, m_Position.z);
	if (ImGui::Button("bom"))
	{
		state=BATTLE;
	}
	ImGui::End();
}

void Enemy::Damage(float damage)
{
	
	if (!hit)
	{
		GetComponent<StateMachine>()->changeState(GetComponent<EStateDamage>());
		HP -= damage;
		if (HP < 0)
		{
			this->SetDestroy();
		}
		hit = true;
	}
}

void Enemy::HitReset()
{
	hit = false;
}

bool Enemy::SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length)
{
	//�v���C���[�Ǝ��g�����ԃx�N�g��
	Vector3 vecobj;
	vecobj = playerpos - m_pos;

	//�v���C���[�Ƃ̋��������߂�
	float objlength = vecobj.Length();

	//���e�����͈̔͊O���`�F�b�N
	if (objlength > length)
	{
		return false;
	}

	//�����x�N�g��
	Vector3 vecview;
	vecview = playerpos - m_pos;

	//���K��
	vecview.Normalize();
	vecobj.Normalize();

	//���ς��v�Z
	float dotobj = vecview.Dot(vecobj);

	//��]
	Matrix mtx;
	mtx = mtx.CreateRotationZ(fov / 2.0f);

	Vector3 vecrotview;
	vecrotview = vecview.Transform(vecview, mtx);

	float dotrotview = vecview.Dot(vecrotview);

	if (dotrotview <= dotobj)
	{
		return true;
	}

	return false;
}


void Enemy::SetAnimName2(const char* _Name)
{
	assert(m_Model->CheckAnimData("_Name") == false && "�w��̃A�j���[�V������������܂���ł����B�����̖��O��A�f�[�^�������Ă��邩�m�F���Ă��������B");

	//�Đ��A�j���[�V�����̕ύX�A�e�A�j���[�V�����֘A�̕ϐ��̃��Z�b�g
	m_Animname1 = m_Animname2;
	m_Animname2 = _Name;
	m_Frame1 = m_Frame2;
	m_Frame2 = 0;
	m_BlendRate = 0.0;
}