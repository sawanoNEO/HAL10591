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
	Vector3 s = player->GetSide();
	Vector3 pos = player->GetPosition();
	PLAYERSTATE state = player->GetPstate();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	float ST = player->GetST();
	bool Wait = player->GetWait();
	
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
			//ここからプレイヤーの回転処理
			Vector3 currentRot = player->GetRotation();
			Vector3 kakudo = XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX)));
			float Cta = atan2(kakudo.x, kakudo.z);
			currentRot.y = Cta;
			player->SetRotation(currentRot);
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

			//ここからプレイヤーの回転処理
			Vector3 currentRot = player->GetRotation();
			Vector3 kakudo = XMVector3Normalize(camera->VecYRemove(cameraf)*Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras)*Input::GetStick(Input::LeftX)));
			float Cta = atan2(kakudo.x, kakudo.z); //目標の角度
			if (abs(Cta-currentRot.y)>4)
			{
				if (Cta < 0)
				{
					Cta = -Cta;
				}
				else if (Cta > 0)
				{
					Cta = currentRot.y-Cta;
				}
			}
			
			if (rotationThreshold > abs(Cta - currentRot.y))
			{
				currentRot.y = Cta;
			}
			else if (Cta > currentRot.y)
			{
				currentRot.y += 0.3;
			}
			else if (Cta < currentRot.y)
			{
				currentRot.y -= 0.3;
			}
			player->SetRotation(currentRot);
			//ここまで

			Accel += Accel * 0.1;//加速度の加算
			DAccel = 150.0;

			state = NONE;
		}
		else //何も行動していないとき
		{
			Accel =200.0;//加速の値を初期値に戻す
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

	if (Input::GetController(Input::x, Input::HELD))
	{
		Vector3 rot = player->GetRotation();
		rot.y += 0.01f;
		player->SetRotation(rot);
	}

	if (camera->GetRock())
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
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	cameras = camera->VecYRemove(cameras);
	cameraf = camera->VecYRemove(cameraf);
	Vector3 a = player->GetRotation();
	Vector3 kakudo = XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX)));
	float Cta = atan2(kakudo.x, kakudo.z); //目標の角度

	ImGui::Begin("MobeComponent");
	ImGui::Text("Cta=%f", Cta);
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
