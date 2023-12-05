#include "StateAttack.h"
#include "Colider.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/player.h"

using namespace DirectX::SimpleMath;

void StateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	AttackObj = scene->AddGameObject<GameObject>(1);
	AttackObj->AddComponent<Colider>();
	AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
}

void StateAttack::Enter()
{
}

void StateAttack::Exit()
{
	Vector3 forward = m_GameObject->GetForward();
}

void StateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();


}

void StateAttack::Draw()
{
}
