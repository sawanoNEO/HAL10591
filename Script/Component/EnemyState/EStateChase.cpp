#include "EStateChase.h"
#include "EStateNone.h"
#include "EStateAttack.h"

#include "../Rigidbody.h"
#include "../Colider.h"

#include "../../System/manager.h"
#include "../../Scene/scene.h"

#include "../../GameObject/enemy.h"
#include "../../GameObject/player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EStateChase::Enter()
{
	m_GameObject->SetAnimName2("Walk");
}

void EStateChase::Exit()
{
}

void EStateChase::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();
	Vector3 forward = m_GameObject->GetForward();
	Colider* colider = m_GameObject->GetComponent<Colider>();
	Colider* playercolider = player->GetComponent<Colider>();
	Vector3 rotation = m_GameObject->GetRotation();
	Vector3 vec = player->GetPosition() - m_GameObject->GetPosition();

	if (vec.Length() > 15.0f)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
	else if (vec.Length() > 3.0f)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateAttack>());
	}

	rotation.y = atan2(vec.x, vec.z);
	forward = XMVector3Normalize(player->GetPosition() - m_GameObject->GetPosition());
	
	m_GameObject->SetRotation(rotation);
	rb->AddForce(forward * 200.0, ForceMode::Force);


}

void EStateChase::Draw()
{
}
