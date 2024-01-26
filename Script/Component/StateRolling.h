#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateRolling :  public State
{
private:
	int m_Startup = 4;   //���[�����O(���)�̖��G���Ԃ���������܂ł̎���(�t���[��)
	int m_Invincible = 47; //���G����
	int m_Recovery = 18;   //���[�����O�̌�ɍs���\�ɂȂ�܂ł̎���(�t���[��)
	int m_Count;       //��𐬗�����ǂꂾ���̃t���[�����o������

	bool m_KeepABurron = false;//��ԑJ�ڌォ��A�{�^�����������ςȂ����ǂ���

	DirectX::SimpleMath::Vector3 m_Rolvec;   //���[�����O�̕��������߂�ϐ�

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

