#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateWaitandSee : public State
{
private:

	int m_HowLong;//�l�q�������鎞��(�t���[��)
	int m_Count;//�o�߃t���[��
	float m_Distance=5.0f; //m_BackPoint�����肷��ۂ̋��������߂�
	DirectX::SimpleMath::Vector3 m_BackPoint;//�v���C���[���炠����x���ꂽ�����̓_
	DirectX::SimpleMath::Vector3 m_WalkDirection;//��������
	
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

