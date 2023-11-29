#include "StateRolling.h"
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

	cnt++;
}

void StateRolling::Draw()
{
}
