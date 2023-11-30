#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateRolling :  public State
{
private:
	int startup = 2;   //���[�����O(���)�̖��G���Ԃ���������܂ł̎���(�t���[��)
	int invincible = 40; //���G����
	int recovery = 15;   //���[�����O�̌�ɍs���\�ɂȂ�܂ł̎���(�t���[��)
	int cnt;       //��𐬗�����ǂꂾ���̃t���[�����o������
	DirectX::SimpleMath::Vector3 Rolvec;   //���[�����O�̕��������߂�ϐ�

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void Draw()override;
};

