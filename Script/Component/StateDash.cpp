#include "StateDash.h"
#include "StateMachine.h"
#include "StateMove.h"
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
}

void StateDash::Exit()
{
}

void StateDash::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();


	player->SetoldPosition(player->GetPosition());
	Vector3 cameraf = camera->GetCamForward();
	Vector3 cameras = camera->GetCamSide();
	Vector3 f = player->GetForward();   //�O�����x�N�g���擾
	Vector3 fNormalR = player->GetSide();
	Vector3 fNormalL = -player->GetSide();
	Vector3 s = player->GetSide();
	Vector3 pos = player->GetPosition();
	Vector3 currentRot = player->GetRotation();//��]�擾
	PLAYERSTATE state = player->GetPstate();
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	float ST = player->GetST();
	bool Wait = player->GetWait();

	if (Input::GetController(Input::a,Input::HELD))
	{
		Vector3 vec = XMVector3Normalize(camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY))) * DAccel;
		rb->AddForce(vec, ForceMode::Acceleration);              ///�����x�����Ƀv���C���[�Ɉړ��̗͂�^����
		//�v���C���[������������ݒ肷��
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(cameraf) * Input::GetStick(Input::LeftY) + (camera->VecYRemove(cameras) * Input::GetStick(Input::LeftX))));
		player->STUse(0.2);//�X�^�~�i����
		DAccel += DAccel * 0.1;//�����x�̉��Z
		if (DAccel >= 250)
		{
			DAccel = 250;
		}
	}
	else if (Input::GetController(Input::a, Input::RELEASED))
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateMove>());
	}
}

void StateDash::Draw()
{
}