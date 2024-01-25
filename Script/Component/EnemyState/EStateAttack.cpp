#include "EStateAttack.h"
#include "EStateChase.h"

#include "../Rigidbody.h"
#include "../Colider.h"

#include "../../GameObject/gameobject.h"
#include "../../GameObject/player.h"

#include "../../Scene/scene.h"
#include "../../System/manager.h"

#include "../../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void EStateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	AttackObj = scene->AddGameObject<GameObject>(1);
	AttackObj->AddComponent<Colider>();
	AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	Startup = 22;
	ActiveFrames = 5;
	Recovery = 21;
	cnt = 0;
	Power = 45;
}

void EStateAttack::Enter()
{
	m_GameObject->SetAnimName2("Attack");
	cnt = 0;
	hit = false;
}

void EStateAttack::Exit()
{
}

void EStateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = m_GameObject->GetForward();
	Vector3 pos = player->GetPosition();
	bool hitCheck = false;
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();
	Colider* col = player->GetComponent<Colider>();

	AttackObj->SetPosition(m_GameObject->GetPosition() + forward * 3);

	if (cnt < Startup)
	{
	}
	else if (cnt < Startup + ActiveFrames)/////攻撃判定が出ている時間。持続部分。
	{
		if (cnt == Startup)
		{
			Vector3 vec = forward * 100;
			rb->AddForce(vec, ForceMode::Impulse);
		}

		//攻撃が当たっているか判定する
		if (AttackObj->GetComponent<Colider>()->CollisionAABB(AttackObj->GetComponent<Colider>()->GetAABB(), col)->GetTug() == PLAYER)
		{
			player->Damage(Power);
		}
	}
	//else if (cnt < Startup + ActiveFrames + Recovery)         //攻撃後の硬直時間。この間の一定のタイミングでもういちど攻撃ボタンを押すと連撃になる
	//{
	//	if (Input::GetController(Input::R1, Input::PRESSED, 15) != -1 &&
	//		player->GetST() > 0.0f &&
	//		cnt > Startup + ActiveFrames + (Recovery / 3))     //硬直が始まってすぐは連撃に移行できない
	//	{
	//		cnt = Startup - 10;
	//		player->SetFrame1(0);
	//		//player->STUse(17.0f);
	//	}
	//}
	
	cnt++;
}

void EStateAttack::StateChange()
{
	if (cnt > Startup + ActiveFrames + Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
	}

}

void EStateAttack::Draw()
{
	ImGui::Begin("EnemyAttaState");
	ImGui::SliderInt("Frame", &Recovery, 0, 50);
	ImGui::End();
}