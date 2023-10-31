#include "Rolling.h"
#include "input.h"
#include "manager.h"
#include "scene.h"

#include "player.h"
#include "Rigidbody.h"
#include "camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Rolling::Init()
{
	startup = 2;
	invincible = 17;
	recovery = 15;
	reception = 20;
	cnt = 0;
	receptioncnt = 0;
}

void Rolling::Update()
{
	//処理に必要なオブジェクトやコンポーネントを取得する
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	PLAYERSTATE pstate=NONE;   ///プレイヤーの状態
	Vector3 forward;
	Rigidbody* rb;
	Camera* camera;
	Vector3 camforward;//カメラの前向きベクトル
	Vector3 camside;   //カメラの横向きベクトル

	if (player)
	{
		pstate = player->GetPstate();
		forward = player->GetForward();
		rb = player->GetComponent<Rigidbody>();
		camera = scene->GetGameObject<Camera>();
		camforward = camera->GetCamForward();
		camside = camera->GetCamSide();

		if (pstate == DASH)
		{
			receptioncnt++;
		}
		
		if (pstate == NONE &&     //プレイヤーの操作がプレーンな状態で
			Input::GetController(Input::a, Input::RELEASED) &&          //回避ボタンを離す
			receptioncnt < reception&&                                  //回避受付時間内である
			player->GetST()>10.0)    /////回避(ダッシュ)ボタンを押してからreceptionで設定されているフレームまでは回避を受け付ける
		{
			player->STUse(16.0);     //スタミナを消費する
			cnt++;
			pstate = ROLLING;//状態をローリングにする
		}
		else if (pstate==ROLLING && cnt < startup)                  //ローリング(バックステップ)の前隙
		{
			cnt++;
			if (!Input::GetStickState())              //スティックによる入力をしていなかったらバックステップになる
			{
				pstate = BACKSTEP;
			}
			else
			{
				pstate = ROLLING;
			}

		}
		else if ((pstate == ROLLING ||pstate==BACKSTEP)&& cnt < startup + invincible)      //無敵時間
		{
			if (cnt == startup)              //無敵時間に入った最初のフレームだけ大きな力を加える
			{
				if (pstate==BACKSTEP)              //スティックによる入力をしていなかったらバックステップになる
				{
					Vector3 vec =forward * -100;
					rb->AddForce(vec, ForceMode::Impulse);              //プレイヤーに移動の力を与える
					//rb->AddForce(-forward*100, ForceMode::Impulse);
				}
				else
				{
					Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//ローリングの方向を決定
					rb->AddForce(Rolvec*250, ForceMode::Impulse);              //プレイヤーに移動の力を与える
					//rb->AddForce(forward*100, ForceMode::Impulse);
				}
			}
			else if (cnt > startup)
			{
				if (pstate == BACKSTEP) //以降は弱い力を加え続ける
				{
					rb->AddForce(-200 * forward, ForceMode::Acceleration);
				}
				else
				{
					rb->AddForce(Rolvec*150, ForceMode::Acceleration);
				}
			}
			cnt++;
		}
		else if ((pstate == ROLLING ||pstate==BACKSTEP)&& cnt < startup + invincible + recovery)//ローリングの後隙
		{
			cnt++;
			////////////後隙中の一定時間は回避、攻撃が可能
			if ((pstate == ROLLING || pstate == BACKSTEP) && cnt > startup + invincible+7)
			{
				if (Input::GetController(Input::a, Input::UP))
				{
					receptioncnt = 0;
				}
				else if (Input::GetController(Input::a, Input::HELD))
				{
					receptioncnt++;
				}
				else if (Input::GetController(Input::a, Input::RELEASED) && receptioncnt < reception&& player->GetST()>10.0)
				{
					cnt = 1;
					player->STUse(16.0);
				}
			}
		}
		else if ((pstate == ROLLING||pstate==BACKSTEP) && cnt >= startup + invincible + recovery)
		{
			pstate = NONE;
			cnt = 0;
			receptioncnt = 0;
		}

		if(pstate==NONE&&Input::GetController(Input::a,Input::UP))
		{
			receptioncnt = 0;
		}
		else if (Input::GetController(Input::a, Input::HELD))
		{
			receptioncnt++;
		}

		//if (pstate == DASH)
		//{
		//	receptioncnt++;
		//}

		//if (Input::GetController(Input::a, Input::RELEASED) && (pstate == NONE || pstate == DASH)&&receptioncnt<reception&&cnt==0)
		//{
		//	//Vector3 vec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))) * 2000;
		//	//rb->AddForce(vec, ForceMode::Impulse);              //プレイヤーに移動の力を与える
		//	if (!Input::GetStickState())              //スティックによる入力をしていなかったらバックステップになる
		//	{
		//		pstate = BACKSTEP;
		//	}
		//	else
		//	{
		//		pstate = ROLLING;
		//	}
		//	cnt++;
		//}
		//else if ((pstate==BACKSTEP||pstate==ROLLING)&&cnt < startup)///////前隙
		//{
		//	player->STUse(16.0);
		//	if(pstate==BACKSTEP)
		//		rb->AddForce(Vector3(-forward.x * 500, forward.y * 500, -forward.z * 500), ForceMode::Impulse);
		//	else
		//		rb->AddForce(Vector3(forward.x * 100, forward.y * 100, forward.z * 100), ForceMode::Impulse);
		//	cnt++;
		//}
		//else if ((pstate == BACKSTEP || pstate == ROLLING) && cnt < startup + invincible)///////無敵時間
		//{
		//	cnt++;
		//	if (pstate == BACKSTEP)
		//	{
		//		rb->AddForce(-200 * forward, ForceMode::Acceleration);
		//	}
		//	else
		//	{
		//		rb->AddForce(200 * forward, ForceMode::Acceleration);
		//	}
		//}
		//else if ((pstate == BACKSTEP || pstate == ROLLING) && cnt < startup + invincible + recovery)
		//{
		//	cnt++;
		//	if (Input::GetController(Input::a,Input::HELD))
		//	{
		//		if (pstate == ROLLING)
		//		{
		//			receptioncnt = 0;
		//			pstate = NONE;
		//		}
		//		receptioncnt++;
		//	}
		//}
		//else if ((pstate == BACKSTEP || pstate == ROLLING) && cnt >= startup + invincible + recovery)
		//{
		//	cnt = 1;
		//}

		//if (Input::GetController(Input::a, Input::RELEASED))
		//{
		//	receptioncnt = 0;
		//}
		player->SetPstate(pstate);
	}
}
