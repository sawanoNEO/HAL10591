#include "Move.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "imguimanager.h"
#include "manager.h"
#include "camera.h"
#include "Rigidbody.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;


void Move::Update()
{
	///////////////今はこれでいけてるけどそのうち攻撃してるのに走れるとかいう問題がおきそうなので要対策
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();

	OldPos = player->GetPosition();
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	Vector3 f = player->GetForward();   //前向きベクトル取得
	Vector3 fNormalR = player->GetSide();
	Vector3 fNormalL = -player->GetSide();
	Vector3 s = player->GetSide();
	Vector3 pos = player->GetPosition();
	Vector3 currentRot = player->GetRotation();//回転取得
	PLAYERSTATE state = player->GetPstate();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	float ST = player->GetST();
	bool Wait = player->GetWait();

	//回転処理
	if(promissDirection.x * f.x + promissDirection.y * f.y + promissDirection.z * f.z < 0.95&&
		(fNormalR.x*promissDirection.x+fNormalR.z*promissDirection.z)>0)
	{
		currentRot.y += 0.4;
	}
	else if (promissDirection.x * f.x + promissDirection.y * f.y + promissDirection.z * f.z < 0.95 &&
		(fNormalL.x * promissDirection.x + fNormalL.z * promissDirection.z)>0)
	{
		currentRot.y -= 0.4;
	}
	
	player->SetRotation(currentRot);
			//ここまで
	
	if (state == NONE || state ==DASH)
	{
		if (Input::GetController(Input::a, Input::HELD) &&
			(Input::GetController(Input::LeftUP, Input::HELD) ||
				Input::GetController(Input::LeftDown, Input::HELD) ||
				Input::GetController(Input::LeftLeft, Input::HELD) ||
				Input::GetController(Input::LeftRight, Input::HELD)) && ST > 0 && !Wait)        //スティックによる操作が行われていないと加速度をリセットする
		{
			state = DASH;
			/*pos += (XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * DAccel);
			player->SetPosition(pos);
			*/
			Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * DAccel;

			rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
			//プレイヤーが向く方向を設定する
			promissDirection = XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX)));
			player->STUse(0.2);//スタミナ消費
			DAccel += DAccel * 0.1;//加速度の加算
		}
		else if (Input::GetController(Input::LeftUP, Input::HELD) ||
			Input::GetController(Input::LeftDown, Input::HELD) ||
			Input::GetController(Input::LeftLeft, Input::HELD) ||
			Input::GetController(Input::LeftRight, Input::HELD))        //スティックによる操作が行われていないと加速度をリセットする
		{
			//pos += (XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * Accel);

			//変数名はvecだが実際はどの方向にどのぐらいの力を加えるかを計算し、格納している
			Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY)))*Accel;
			rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
			player->SetPosition(pos);

			//プレイヤーが向く方向を設定する
			promissDirection = XMVector3Normalize(camera->VecYRemove(cameraf)*Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras)*Input::GetStick(Input::LeftX)));

			Accel += Accel * 0.1;//加速度の加算
			DAccel = 150.0;

			state = NONE;
		}
		else //何も行動していないとき
		{
			Accel = 150.0;//加速の値を初期値に戻す
			DAccel = 150.0;
			state = NONE;
		}
	}
	if (Accel >= 150)  //基底の量を超えないようにする
	{
		Accel = 150;
	}
	if (DAccel >= 300)
	{
		DAccel = 300;
	}


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
			player->SetPstate(state);
}

void Move::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	Player* player = scene->GetGameObject<Player>();
	Vector3 f = player->GetForward();

	Matrix mat;

	Vector3 fNormalR = player->GetSide();
	Vector3 fNormalL = -player->GetSide();
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	cameras = camera->VecYRemove(cameras);
	cameraf = camera->VecYRemove(cameraf);
	Vector3 a = player->GetRotation();
	float Cta = atan2(promissDirection.x, promissDirection.z); //目標の角度

	float innerProduct = f.x * promissDirection.x + f.z * promissDirection.z;

	ImGui::Begin("MobeComponent");
	ImGui::Text("Cta=%f", Cta);
	ImGui::Text("naiseki=%f", innerProduct);
	ImGui::Text("Forward%f", f.x);
	ImGui::Text("Forward%f", f.y);
	ImGui::Text("Forward%f", f.z);
	ImGui::Text("Right%f", fNormalR.x);
	ImGui::Text("Right%f", fNormalR.y);
	ImGui::Text("Right%f", fNormalR.z);
	ImGui::Text("Left%f", fNormalL.x);
	ImGui::Text("Left%f", fNormalL.y);
	ImGui::Text("Left%f", fNormalL.z);
	//ImGui::Text("%f\n", Input::GetStick(Input::LeftX));
	//ImGui::Text("%f\n", Input::GetStick(Input::LeftY));
	//ImGui::Text("Vecx%f\n", cameraf.x);
	//ImGui::Text("Vecy%f\n", cameraf.y);
	//ImGui::Text("Vecz%f\n", cameraf.z);
	//ImGui::Text("%f\n", cameras.x);
	//ImGui::Text("%f\n", cameras.y);
	//ImGui::Text("%f\n", cameras.z);
	//ImGui::Text("PlayerRot%f\n", a.x);
	ImGui::Text("PlayerRot%f\n", a.y);
	//ImGui::Text("PlayerRot%f\n", a.z);
	ImGui::End();
}
