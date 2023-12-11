#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateRolling :  public State
{
private:
	int startup = 4;   //���[�����O(���)�̖��G���Ԃ���������܂ł̎���(�t���[��)
	int invincible = 47; //���G����
	int recovery = 18;   //���[�����O�̌�ɍs���\�ɂȂ�܂ł̎���(�t���[��)
	int cnt;       //��𐬗�����ǂꂾ���̃t���[�����o������
	DirectX::SimpleMath::Vector3 Rolvec;   //���[�����O�̕��������߂�ϐ�

public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

