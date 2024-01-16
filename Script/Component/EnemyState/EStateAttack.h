#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateAttack : public State
{
private:
	GameObject* AttackObj;
	int Startup;            //�U���̑O���̃t���[����
	int ActiveFrames;       //�U���̎����t���[����
	int Recovery;           //�U���̌㌄�̃t���[����
	int cnt = 0;                //���݂ǂꂾ���U���̃��[�V�������i��ł��邩
	int Power;              //�U����

	bool hit = false;       //��x�̍U���ŕ����q�b�g��������ɂȂ�̂�h���t���O
public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

