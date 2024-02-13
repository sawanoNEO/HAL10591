#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>
#include <vector>

class EStateAttack : public State
{
private:
	GameObject* m_AttackObj;
	class Slash* slash;
	std::vector<int> m_Startup;            //�U���̑O���̃t���[����
	std::vector<int> m_ActiveFrames;       //�U���̎����t���[����
	std::vector<int> m_Recovery;           //�U���̌㌄�̃t���[����
	std::vector<int> m_Power;              //�U����
	std::vector<int> m_DubleAttackRate;    //�A���U���̊m��(%)
	int m_cnt = 0;            //���݂ǂꂾ���U���̃��[�V�������i��ł��邩

	int m_AttackNum = 0;//����A���ōU��������
	bool m_DoubleAttack = false;//�A���U�����o�������ǂ���
	bool m_hit = false;       //��x�̍U���ŕ����q�b�g��������ɂȂ�̂�h���t���O

	int m_SENumber;           //�ǂ̉����Đ����邩
public:
	using State::State;
	void Init()override;
	void Init(int _startup, int _activeframes, int _recovery, int _power, int _doubleattackrate, DirectX::SimpleMath::Vector3 _scale); 
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

