#include "StateAttack.h"
#include "StateNone.h"
#include "Colider.h"
#include "Rigidbody.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/player.h"
#include "../GameObject/enemy.h"
#include "../GameObject/camera.h"

using namespace DirectX::SimpleMath;

void StateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	AttackObj = scene->AddGameObject<GameObject>(1);
	AttackObj->AddComponent<Colider>();
	AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	Maxcombo = 2;
	combo = 0;
	Startup = 32;
	ActiveFrames = 5;
	Recovery = 35;
	cnt = 0;
	Power = 45;
	STconsumption = 17.0;
}

void StateAttack::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimName2("Attack");
	cnt = 0;

}

void StateAttack::Exit()
{
}

void StateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = player->GetForward();
	std::vector<Enemy*> enemys = scene->GetGameObjects<Enemy>();
	std::vector<Colider*> col;//オブジェクトの当たり判定を取得する
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	bool hitCheck = false;
	Rigidbody* rb = player->GetComponent<Rigidbody>();

	AttackObj->SetPosition(player->GetPosition()+forward*3);

	col.resize(enemys.size());
	for (int i = 0; i < enemys.size(); i++)
	{
		col[i] = enemys[i]->GetComponent<Colider>();
	}

	if (cnt < Startup)
	{

		STconsumption = 17.0f;

		for (const auto enemy : enemys)
		{
			enemy->HitReset();
		}
		if (cnt == 1)
		{
			player->STUse(STconsumption);
			Vector3 vec = player->GetpromissDirection() * 100.0;
			rb->AddForce(vec, ForceMode::Impulse);
		}
	}
	else if (cnt < Startup + ActiveFrames)/////攻撃判定が出ている時間。持続部分。
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));

		if (enemys.size() == 0)
		{
			return;
		}
		for (int i = 0; i < enemys.size(); i++)
		{
			Colider hit = col[i]->CollisionAABB(AttackObj->GetComponent<Colider>()->GetAABB(), col[i]);
			if (hit.GetTug() == ENEMY)
			{
				if (enemys[i] && !hitCheck)
				{
					enemys[i]->Damage(Power);
				}
			}
		}
	}
	else if (cnt < Startup + ActiveFrames + Recovery)         //攻撃後の硬直時間。この間の一定のタイミングでもういちど攻撃ボタンを押すと連撃になる
	{
		if (Input::GetController(Input::R1, Input::PRESSED,15)!=-1 &&
			player->GetST() > 0.0f &&
			cnt > Startup + ActiveFrames + (Recovery / 3))     //硬直が始まってすぐは連撃に移行できない
		{
			cnt = Startup - 10;
			//player->STUse(17.0f);
		}
	}
	else if (cnt > Startup + ActiveFrames + Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}

	cnt++;
}

void StateAttack::Draw()
{
}
