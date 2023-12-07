#include "EStateNone.h"
#include "../../GameObject/gameObject.h"

void EStateNone::Enter()
{
	m_GameObject->SetAnimName2("Idle");

}

void EStateNone::Exit()
{
}

void EStateNone::StateUpdate()
{
}

void EStateNone::Draw()
{
}
