#include "StateDash.h"
#include "StateItem.h"
#include "StateMove.h"
#include "StateNone.h"
#include "Rigidbody.h"

#include "../System/manager.h"
#include "../System/input.h"

#include "../Scene/scene.h"

#include "../GameObject/camera.h"
#include "../GameObject/player.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void StateDash::Enter()
{
	DAccel = 150.0;
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimSpeed(1.5f);
}

void StateDash::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimSpeed(1.0f);
}

void StateDash::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();


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
	bool Wait = player->GetWait();

	if (Input::GetController(Input::a,Input::HELD))
	{
		Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * DAccel;
		rb->AddForce(vec, ForceMode::Acceleration);              ///加速度を元にプレイヤーに移動の力を与える
		//プレイヤーが向く方向を設定する
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX))));
		player->STUse(0.2);//スタミナ消費
		DAccel += DAccel * 0.1;//加速度の加算
		if (DAccel >= 250)
		{
			DAccel = 250;
		}
	}
}

void StateDash::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	float stamina = player->GetST();

	if (Input::GetController(Input::a, Input::RELEASED)|| stamina <= 0)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateMove>());
	}
	else if (Input::GetController(Input::x, Input::PRESSED))
	{
		//状態をアイテム使用に変更
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateItem>());
	}
	else if (Input::GetStickState() == false)
	{
		//状態を無操作状態に変更
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateDash::Draw()
{
}
