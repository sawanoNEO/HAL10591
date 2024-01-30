#include "Rolling.h"
#include "../System/input.h"
#include "../System/manager.h"
#include "../Scene/scene.h"

#include "../GameObject/player.h"
#include "../Component/Rigidbody.h"
#include "../GameObject/camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Rolling::Init()
{
	startup = 2;
	invincible = 17;
	recovery = 15;
	m_Reception = 20;
	cnt = 0;
	m_Receptioncnt = 0;
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
			m_Receptioncnt++;
		}
		
		if (pstate == NONE &&     //プレイヤーが特殊な状態でない
			Input::GetController(Input::a, Input::RELEASED) &&          //回避ボタンを離す
			m_Receptioncnt < m_Reception&&                                  //回避受付時間内である
			player->GetST()>10.0)    /////回避(ダッシュ)ボタンを押してからm_Receptionで設定されているフレームまでは回避を受け付ける
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
					Vector3 vec =-forward * 100;
					rb->AddForce(vec, ForceMode::Impulse);              //プレイヤーに移動の力を与える
					//rb->AddForce(-forward*100, ForceMode::Impulse);
				}
				else
				{
					Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//ローリングの方向を決定
					rb->AddForce(Rolvec*150, ForceMode::Impulse);              //プレイヤーに移動の力を与える
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
			////////////後隙中の一定時間は再度回避、攻撃が可能
			if ((pstate == ROLLING || pstate == BACKSTEP) && cnt > startup + invincible+7)
			{
				//先行入力を実装した2023/11/10
				if (Input::GetController(Input::a, Input::RELEASED, 20) > 0 && m_Receptioncnt < m_Reception && player->GetST()>10.0)
				{
					cnt = 1;
					player->STUse(16.0);
				}
				else if (Input::GetController(Input::a, Input::UP))
				{
					m_Receptioncnt = 0;
				}
				else if (Input::GetController(Input::a, Input::HELD))
				{
					m_Receptioncnt++;
				}
				//先行入力実装前比較用
				//if (Input::GetController(Input::a, Input::UP))
				//{
				//	m_Receptioncnt = 0;
				//}
				//else if (Input::GetController(Input::a, Input::HELD))
				//{
				//	m_Receptioncnt++;
				//}
				//else if (Input::GetController(Input::a, Input::RELEASED) && m_Receptioncnt < m_Reception&& player->GetST()>10.0)
				//{
				//	cnt = 1;
				//	player->STUse(16.0);
				//}
			}
			else
			{
				//後隙の終了前にスティックによる入力をしていればある程度移動量を残す(回避の度に停止すると気持ちよくない)
				rb->AddForce(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))) * 150, ForceMode::Acceleration);
				player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));
			}
		}
		else if ((pstate == ROLLING||pstate==BACKSTEP) && cnt >= startup + invincible + recovery)//後隙も終了し、各変数のリセットを行う
		{
			pstate = NONE;
			cnt = 0;
			m_Receptioncnt = 0;
            //後隙終了時にスティックによる入力をしていたならある程度移動量を保存する
			rb->AddForce(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)))*150, ForceMode::Acceleration);
			player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));

		}

		if(pstate==NONE&&Input::GetController(Input::a,Input::UP))
		{
			m_Receptioncnt = 0;
		}
		else if (Input::GetController(Input::a, Input::HELD))
		{
			m_Receptioncnt++;
		}
		player->SetPstate(pstate);
	}
}
