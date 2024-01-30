#pragma once
#include "StateMachine.h"

class StateAttack : public State
{
private:
	GameObject* AttackObj;
	
	class Slash* slash;

	int m_Maxcombo;           //�A���ōU�������Ƃ��̃��[�V�����̐�
	int m_combo;              //���݂̍ő�R���{
	int m_Startup;            //�U���̑O���̃t���[����
	int m_ActiveFrames;       //�U���̎����t���[����
	int m_Recovery;           //�U���̌㌄�̃t���[����
	int m_Count;                //���݂ǂꂾ���U���̃��[�V�������i��ł��邩
	int m_Power;              //�U����
	float m_Staminaconsumption;    //�����X�^�~�i�̗�

public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

