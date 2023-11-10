#include "Sword.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"

#include "manager.h"
//#include "Colider.h"

#include "Box.h"
#include <vector>
#include "imguimanager.h"

#include"Rigidbody.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Sword::Init()
{
	Maxcombo = 2;
	combo = 0;
	Startup = 32;
	ActiveFrames = 5;
	Recovery = 35;
	cnt = 0;
	Power = 45;
	STconsumption = 17.0;
}

void Sword::Update()
{
	////処理に必要な情報を取得
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	Vector3 forward = player->GetForward();
	PLAYERSTATE state = player->GetPstate();
	Colider* Pcol = player->Getcolattack();
	Vector3 position = pos;
	position += 0.2 * forward;
	//position.x = pos.x * (forward.x * 1.5);
	//position.z = pos.z * (forward.z * 1.5);
	Pcol->SetAABB(position, 2.0f, 2.0f, 2.0f);//攻撃判定設定
	ab = Pcol->GetAABB();
	std::vector<Enemy*> gameobject=scene->GetGameObjects<Enemy>();//goはGameObjectの意
	std::vector<Colider*> col;//オブジェクトの当たり判定を取得するための配列
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	bool hitCheck = false;

	////ここまで

	col.resize(gameobject.size());
	for (int i = 0; i < gameobject.size(); i++)
	{
		col[i] = gameobject[i]->GetComponent<Colider>();
	}
	
	if (Input::GetController(Input::R2, Input::PRESSED) && state != ATTACK && player->GetST() > 10.0f)//////////強攻撃
	{
		cnt++;
		state = ATTACK; 
		STconsumption = 25.0f;
		Startup = 40;
		Power = 60;
	}
	if (Input::GetController(Input::R1, Input::PRESSED)&&state!=ATTACK&&player->GetST()>10.0f)////プレイヤーの状態遷移
	{
		cnt++;
		state = ATTACK;
		STconsumption = 17.0f;

		for (const auto enemy : gameobject)
		{
			enemy->HitReset();
		}
		
	}
	if (state==ATTACK&&cnt < Startup)     ////攻撃ボタンを押された瞬間から攻撃判定が出るまでの隙(ちょっと前に踏み込む)
	{
		/*pos += 0.01 * forward;
		player->SetPosition(pos);*/
		cnt++;
	}
	else if (state==ATTACK&&cnt < Startup + ActiveFrames)/////攻撃判定が出ている時間。持続部分。
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));
		if (cnt == Startup)
		{
			player->STUse(STconsumption);
			Vector3 vec = player->GetpromissDirection() * 100.0;
			rb->AddForce(vec, ForceMode::Impulse);
		}
		cnt++;

		if (gameobject.size() == 0)
			return;

		for (int i = 0; i < gameobject.size(); i++)
		{
			Colider hit = col[i]->CollisionAABB(Pcol->GetAABB(), col[i]);
			if ( hit.GetTug() == ENEMY)
			{
				if (gameobject[i] && !hitCheck)
				{
					hitCheck = true;
					gameobject[i]->Damage(Power);
					//go[i]->SetDestroy();
				}
			}
		}
	}
	else if (state == ATTACK && cnt < Startup + ActiveFrames + Recovery)         //攻撃後の硬直時間。この間の一定のタイミングでもういちど攻撃ボタンを押すと連撃になる
	{
		cnt++;
		if (Input::GetController(Input::R1, Input::PRESSED)&&
			player->GetST()>0.0f&&
			cnt>Startup+ActiveFrames+(Recovery/3))     //硬直が始まってすぐは連撃に移行できない
		{
			cnt = Startup - 10;
			//player->STUse(17.0f);
		}
	}
	else if(state == ATTACK)
	{
		cnt = 0;
		state = NONE;
	}

	player->SetPstate(state);//ぷれいやーの状態を反映
}

void Sword::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Colider* Pcol = player->Getcolattack();
	AABB2 aabb = Pcol->GetAABB();
	ImGui::Begin("Sword");
	ImGui::Text("MaxX=%f\n",ab.max.x);
	ImGui::Text("MinX=%f\n",ab.min.x);
	ImGui::Text("MaxZ=%f\n",ab.max.z);
	ImGui::Text("MinZ=%f\n",ab.min.z);
	ImGui::End();
}
