#include "Est.h"

#include "../System/manager.h"
#include "../Scene/scene.h"
#include "../GameObject/player.h"
#include "../GameObject/camera.h"
#include "../Component/Rigidbody.h"
#include "../Component/Move.h"

#include "../System/input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Est::Init()
{
	SetWait(31);    //発生及び硬直を設定
	SetRecovery(33);
	cnt = 0;
	quantity = maxquantity;
}

void Est::Update()
{
	Scene* scene = Manager::GetScene();//現在シーン
	Player* player = scene->GetGameObject<Player>();// プレイヤー取得
	PLAYERSTATE pstate = player->GetPstate();   //状態取得
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	Vector3 pos = player->GetPosition();
	/////////////////////臨時
	//float acc = player->GetComponent<Move>()->GetAccel();

	int wait = GetWait();
	int recovery = GetRecovery();

	if (pstate == ITEM)// アイテムを使用している状態の処理
	{
		Vector3 currentRot = player->GetRotation();
		Vector3 kakudo = XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX)));

		float Cta = atan2(kakudo.x, kakudo.z);
		currentRot.y = Cta;
		player->SetRotation(currentRot);
		/////////////////////////臨時
		//Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * acc;
		//rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
		//↑要修正2023/10/26
		//acc = acc * 0.9;
		//if (acc < 20)
		//{
		//	acc = 20;
		//}
		//player->GetComponent<Move>()->SetAccel(acc);
		
		if (camera->GetRock())//ターゲットカメラ状態の時の処理
		{
			Vector3 currentRot = player->GetRotation();
			Vector3 enemypos = camera->GetRockEnemy()->GetPosition();
			float fx = enemypos.x - pos.x;
			float fz = enemypos.z - pos.z;
			Vector3 nom = { fx,0.0,fz };
			nom.Normalize();
			float afo = atan2(nom.x, nom.z);
			currentRot.y = afo;
			player->SetRotation(currentRot);
			/*Vector3 rockPos = camera->GetRockEnemy()->GetPosition();
			forward = XMVector3Normalize(rockPos - m_Position);
			SetRotation(forward);*/
		}
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