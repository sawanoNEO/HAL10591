#include "Est.h"

#include "manager.h"
#include "scene.h"
#include "player.h"
#include "camera.h"
#include "Rigidbody.h"
#include "Move.h"

#include "input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Est::Init()
{
	SetWait(31);    //発生、硬直はダクソ3のものを引用(推測)
	SetRecovery(33);
	cnt = 0;
	quantity = maxquantity;
}

void Est::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	PLAYERSTATE pstate = player->GetPstate();
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	Vector3 pos = player->GetPosition();
	float acc = player->GetComponent<Move>()->GetAccel();

	int wait = GetWait();
	int recovery = GetRecovery();

	if (pstate == ITEM)// アイテムを使用している状態の処理
	{
		Vector3 currentRot = player->GetRotation();
		Vector3 kakudo = XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX)));
		float Cta = atan2(kakudo.x, kakudo.z);
		currentRot.y = Cta;
		player->SetRotation(currentRot);
		Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * acc;
		rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
		player->SetPosition(pos);
		acc = acc * 0.9;
		if (acc < 20)
		{
			acc = 20;
		}
		player->GetComponent<Move>()->SetAccel(acc);
	}

	if (Input::GetController(Input::x, Input::PRESSED) && (pstate == NONE||pstate==DASH))
	{
		pstate = ITEM;
	}

	if (pstate == ITEM && cnt < wait)
	{
		cnt++;
	}
	else if (pstate == ITEM && cnt == wait)
	{
		cnt++;
		player->ASHP(650);
	}
	else if (pstate == ITEM && cnt < wait + recovery)
	{
		cnt++;
	}
	else if (pstate == ITEM && cnt >= wait + recovery)
	{
		cnt = 0;
		pstate = NONE;
	}


	player->SetPstate(pstate);
}