#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>
class EStateNone : public State
{
private:

	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//����͈̓`�F�b�N�֐��B

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;

};

