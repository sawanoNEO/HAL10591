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

#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../Component/Rigidbody.h"

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Boss::Init()
{
	MaxHP = 1000.0;
	HP = MaxHP;
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset\\model\\Boss\\Boss.fbx");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossIdle.fbx", "BossIdle");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossRun.fbx", "BossRun");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossDamage.fbx", "BossImpact");
	m_Model->LoadAnimation("asset\\model\\Boss\\BossPuntch.fbx", "BossAttack");
	//m_Model->LoadAnimation("asset\\model\\Player\\Slash3.fbx", "Attack3");
	//m_Model->LoadAnimation("asset\\model\\Player\\EnemyRightStrafe.fbx", "EnemyRightStrafe");
	//m_Model->LoadAnimation("asset\\model\\Player\\EnemyLeftStrafe.fbx", "EnemyLeftStrafe");
	m_Model->LoadAnimation("asset\\model\\PowerUp.fbx", "BossAppearance");
	SetAnimName2("BossAppearance");
	SetAnimName2("BossAppearance");
	SetAnimSpeed(0.5f);
	/*AddComponent<EStateNone>();
	AddComponent<EStateDamage>();
	AddComponent<EStateChase>();
	AddComponent<EStateAttack>();
	AddComponent<EStateWaitandSee>();
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<EStateNone>());*/

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
	const char* Animname1 = m_Animname1.c_str();//アニメーションの名前1
	const char* Animname2 = m_Animname2.c_str();//アニメーションの名前2

	//HPバーの表示位置
	//m_HP->deliverParamater(HP);
	//m_HP->SetPosition(Vector3(m_Position.x, m_Position.y + 5.0f, m_Position.z));


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
	//位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && vel.y < 0.0f)
	{
		m_Position.y = groundHeight;       //床の高さで止まる
		vel.y = 0.0f;                      //落ちる速度をリセット
		acc = force / mass;                //加速度を摩擦で減少させる
		rb->SetAccel(acc);
		rb->SetVelocity(vel);
	}

	//敵と接触していた場合に移動度を0にする処理
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

	//アニメーションのフレームを進める
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

void Boss::Damage(float)
{
}

void Boss::HitReset()
{
}

void Boss::SetAnimName2(const char* _Name)
{
	assert(m_Model->CheckAnimData(_Name) != false && "指定のアニメーションが見つかりませんでした。引数の名前や、データが入っているか確認してください。");

	//再生アニメーションの変更、各アニメーション関連の変数のリセット
	m_Animname1 = m_Animname2;
	m_Animname2 = _Name;
	m_Frame1 = m_Frame2;
	m_Frame2 = 0;
	m_BlendRate = 0.0;
}
