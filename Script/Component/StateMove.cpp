#include "StateMove.h"
#include "StateDash.h"
#include "StateRolling.h"
#include "StateAttack.h"

#include "../System/input.h"
#include "../GameObject/player.h"
#include "../Scene/scene.h"
#include "../ImGui/imguimanager.h"
#include "../System/manager.h"
#include "../GameObject/camera.h"
#include "../Component/Rigidbody.h"
#include "../Component/animationModel.h"
#include "StateNone.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void StateMove::Enter()
{
	if (m_GameObject->GetComponent<StateMachine>()->GetPastStateName() != "Dash")
	{
		m_GameObject->SetAnimName2("Run");
	}
	Accel = 70.0;//加速の値を初期値に戻す
	DAccel = 150.0;
}

void StateMove::Exit()
{
	receptionCount = 0;
}

void StateMove::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	AnimationModel* anim = player->GetComponent<AnimationModel>();

	player->SetoldPosition(player->GetPosition());
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

	player->STRecover();

	player->SetRotation(currentRot);
	//ここまで

	
	if (Input::GetController(Input::a, Input::HELD) &&
		Input::GetStickState() && ST > 0 && !Wait)        //スティックによる操作が行われていないと加速度をリセットする
	{
		receptionCount++;
		if (receptionCount > reception)//猶予以上Aボタンが押されていればダッシュ状態に移行
		{
			m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateDash>());
		}
	}
	else if (Input::GetController(Input::a, Input::RELEASED) && ST > 0 && !Wait)
	{
		//回避に派生
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateRolling>());
	}
	if (Input::GetController(Input::LeftUP, Input::HELD) ||
		Input::GetController(Input::LeftDown, Input::HELD) ||
		Input::GetController(Input::LeftLeft, Input::HELD) ||
		Input::GetController(Input::LeftRight, Input::HELD))        //スティックによる操作が行われていないと加速度をリセットする
	{
		//pos += (XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * Accel);

		//変数名はvecだが実際はどの方向にどのぐらいの力を加えるかを計算し、格納している
		Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * Accel;
		rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
		player->SetPosition(pos);

		//プレイヤーが向く方向を設定する
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX))));

		Accel += Accel * 0.1;//加速度の加算
		DAccel = 150.0;

		state = NONE;
	}
	else if (Input::GetStickState() == false)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
	if (Accel >= 150)  //基底の量を超えないようにする
	{
		Accel = 150;
	}
	if (DAccel >= 250)
	{
		DAccel = 250;
	}

	if (Input::GetController(Input::R1, Input::PRESSED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateAttack>());
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
		player->SetpromissDirection(f);
		/*Vector3 rockPos = camera->GetRockEnemy()->GetPosition();
		forward = XMVector3Normalize(rockPos - m_Position);
		SetRotation(forward);*/
	}
}

void StateMove::Draw()
{
}
