#pragma once
#include "StateMachine.h"

class StateAttack : public State
{
private:
	GameObject* AttackObj;
	
	int Maxcombo;           //�A���ōU�������Ƃ��̃��[�V�����̐�
	int combo;              //���݂̍ő�R���{
	int Startup;            //�U���̑O���̃t���[����
	int ActiveFrames;       //�U���̎����t���[����
	int Recovery;           //�U���̌㌄�̃t���[����
	int cnt;                //���݂ǂꂾ���U���̃��[�V�������i��ł��邩
	int Power;              //�U����
	float STconsumption;    //�����X�^�~�i�̗�

public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

