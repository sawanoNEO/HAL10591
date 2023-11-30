#include "StateAttack.h"
#include "../Component/Colider.h"
#include "../GameObject/gameObject.h"

using namespace DirectX::SimpleMath;

void StateAttack::Enter()
{
	//if (m_AttackColider == nullptr)
	//{
	//	m_AttackColider = m_GameObject->AddComponent<Colider>();
	//}
	//m_AttackColider->Init(DEFAULT, Vector3(200.0f, 200.0f, 200.0f));
}

void StateAttack::Exit()
{
	Vector3 forward = m_GameObject->GetForward();
}

void StateAttack::StateUpdate()
{
}

void StateAttack::Draw()
{
}
