#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "../GameObject/enemy.h"
#include "../GameObject/player.h"

#include "../Component/Rigidbody.h"
#include "../Component/shader.h"
#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Enemy::Init()
{
	MaxHP = 100.0;
	HP = MaxHP;
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");

	AddComponent<Shadow>()->SetSize(2.0f);
	AddComponent<Colider>()->Init(ENEMY,this->GetScale());
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
	if (player)
	{
		Pcol = player->GetComponent<Colider>();
		forward = XMVector3Normalize(player->GetPosition() - m_Position);
		playerpos = player->GetPosition();
	}
		//state = NONE;
	switch (state)
	{
	case Enemy::NONE:
		break;
	case Enemy::NORMAL:
		if (SearchPlayer(playerpos,m_Position,30.0f,15.0f) == true)////プレイヤーが視野範囲にいたら状態を遷移する
		{
			state = BATTLE;
		}
		break;
	case Enemy::BATTLE:
		rb->AddForce(forward*200.0, ForceMode::Force);
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
		}
		break;
	default:
		break;
	}
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
}

void Enemy::Draw()
{
	ImGui::Begin("Enemy");
	ImGui::Text("HP=%f\n\n", HP);
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
	//プレイヤーと自身を結ぶベクトル
	Vector3 vecobj;
	vecobj = playerpos - m_pos;

	//プレイヤーとの距離を求める
	float objlength = vecobj.Length();

	//許容距離の範囲外かチェック
	if (objlength > length)
	{
		return false;
	}

	//視線ベクトル
	Vector3 vecview;
	vecview = playerpos - m_pos;

	//正規化
	vecview.Normalize();
	vecobj.Normalize();

	//内積を計算
	float dotobj = vecview.Dot(vecobj);

	//回転
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


