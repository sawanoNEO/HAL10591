#pragma once
#include "../StateMachine.h"
#include <SimpleMath.h>

//�v���C���[�ƈ��̋�����ۂ�����ɗl�q�����s�����
class EStateWaitandSee : public State
{
private:

	int m_HowLong;//�l�q�������鎞��(�t���[��)
	int m_Count=0;//�l�q�����J�n���Ă���̌o�߃t���[��
	float m_Distance=5.0f; //m_BackPoint�����肷��ۂ̋��������߂�
	DirectX::SimpleMath::Vector3 m_BackPoint;//�v���C���[���炠����x���ꂽ�����̓_
	DirectX::SimpleMath::Vector3 m_WalkVec;//��������
	int m_Direction;//�l�q���̍ۂɍ��E�ǂ���ɓ����������肷��ϐ�
	bool m_SideMoveflg = false;//���ړ����I���A���ړ������邩�ǂ���
	int m_RandUpperLimit = 90;//�l�q�������鎞�Ԃ����肷�闐���̏��
	int m_RandLowerLimit = 40;//�l�q�������鎞�Ԃ����肷�闐���̉���
public:
	using State::State;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

