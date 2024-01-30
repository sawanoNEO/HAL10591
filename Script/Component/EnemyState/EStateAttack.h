#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

class EStateAttack : public State
{
private:
	GameObject* m_AttackObj;
	class Slash* slash;
	int m_Startup;            //�U���̑O���̃t���[����
	int m_ActiveFrames;       //�U���̎����t���[����
	int m_Recovery;           //�U���̌㌄�̃t���[����
	int m_cnt = 0;                //���݂ǂꂾ���U���̃��[�V�������i��ł��邩
	int m_Power;              //�U����
	int m_DubleAttackRate=10;    //�A���U���̊m��(%)

	bool m_DoubleAttack = false;//�A���U�����o�������ǂ���
	bool m_hit = false;       //��x�̍U���ŕ����q�b�g��������ɂȂ�̂�h���t���O

	int m_SENumber;           //�ǂ̉����Đ����邩
public:
	using State::State;
	void Init()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

