#pragma once
#include "../Component/component.h"
#include <SimpleMath.h>

class Rolling : public Component
{
private:
	int startup;   //���[�����O(���)�̖��G���Ԃ���������܂ł̎���(�t���[��)
	int invincible; //���G����
	int recovery;   //���[�����O�̌�ɍs���\�ɂȂ�܂ł̎���(�t���[��)
	int m_Reception;  //�_�b�V���{�^���������Ă��痣���܂ł̊Ԃɉ������������P�\�t���[��
	int cnt;
	int m_Receptioncnt; //�P�\�t���[���𔻒f���邽�߂̕ϐ�
	DirectX::SimpleMath::Vector3 Rolvec;   //���[�����O&�o�b�N�X�e�b�v�̕���

public:
	using Component::Component;
	void Init()override;
	void Update()override;
};

