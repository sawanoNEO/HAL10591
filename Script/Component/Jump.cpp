#include "Jump.h"
#include "../GameObject/player.h"
#include "../System/input.h"
#include <SimpleMath.h>
#include "../ImGui/imguimanager.h"
#include "../Scene/scene.h"
#include "../System/manager.h"
#include "../Component/Rigidbody.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Jump::Init()
{
	
}

void Jump::Jumping()
{
}

void Jump::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Rigidbody* rb = player->GetComponent<Rigidbody>();

	Vector3 forward = player->GetForward();

	i=player->GetPstate();
	if ((Input::GetKeyTrigger(VK_SPACE)||Input::GetController(Input::LeftStick,Input::PRESSED))&&player->GetPstate()==DASH) //ダッシュ中に入力するとジャンプする
	{
		player->SetPstate(JUMP);//ジャンプ状態にする
		rb->AddForce(Vector3(forward.x * 1000.0, 200.0f, forward.z * 1000.0f), ForceMode::Impulse);
		player->STUse(15.0);
	}

	if (player->GetPstate() == JUMP)
	{
		//player->SetPstate(NONE);
	}

}

void Jump::Draw()
{
#if _DEBUG
	ImGui::Begin("JumpComponent");
	ImGui::Text("time=%i\n",time );
	ImGui::Text("Player=%i\n",i );
	ImGui::End();
#endif
}
