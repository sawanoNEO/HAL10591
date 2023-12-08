#include "EStateAttack.h"
#include "EStateChase.h"
#include "../../GameObject/gameobject.h"

using namespace DirectX::SimpleMath;

void EStateAttack::Enter()
{
	m_GameObject->SetAnimName2("Attack");
}

void EStateAttack::Exit()
{
}

void EStateAttack::StateUpdate()
{
}

void EStateAttack::Draw()
{
}