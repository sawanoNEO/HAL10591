#include "StateRolling.h"
#include "StateNone.h"
#include "Rigidbody.h"

#include "../System/input.h"
#include "../System/manager.h"
#include "../Scene/scene.h"

#include "../GameObject/player.h"
#include "../GameObject/camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void StateRolling::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	player->STUse(16.0);     //スタミナを消費する
	m_GameObject->SetAnimName2("Rolling");
	cnt = 0;
}

void StateRolling::Exit()
{
	cnt = 0;
}

void StateRolling::StateUpdate()
{
	//処理に必要なオブジェクトやコンポーネントを取得する
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward;
	Rigidbody* rb;
	Camera* camera;
	Vector3 camforward;//カメラの前向きベクトル
	Vector3 camside;   //カメラの横向きベクトル

	forward = player->GetForward();
	rb = player->GetComponent<Rigidbody>();
	camera = scene->GetGameObject<Camera>();
	camforward = camera->GetCamForward();
	camside = camera->GetCamSide();

	if (cnt < startup)
	{
		if (Input::GetStickState())
		{
			Rolvec = XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY)));//ローリングの方向を決定
		}
		else
		{
			Rolvec = player->GetForward();
		}
		rb->AddForce(Rolvec * 75, ForceMode::Impulse);              //プレイヤーに移動の力を与える
	}
	if (cnt > 30)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
	cnt++;
}

void StateRolling::Draw()
{
}
