#pragma once
#include "component.h"
#include <SimpleMath.h>

class Rolling : public Component
{
private:
	int startup;   //���[�����O(���)�̖��G���Ԃ���������܂ł̎���(�t���[��)
	int invincible; //���G����
	int recovery;   //���[�����O�̌�ɍs���\�ɂȂ�܂ł̎���(�t���[��)
	int reception;  //�_�b�V���{�^���������Ă��痣���܂ł̊Ԃɉ������������P�\�t���[��
	int cnt;
	int receptioncnt; //�P�\�t���[���𔻒f���邽�߂̕ϐ�
	DirectX::SimpleMath::Vector3 Rolvec;   //���[�����O&�o�b�N�X�e�b�v�̕��������߂�ϐ�

public:
	using Component::Component;
	void Init()override;
	void Update()override;
};

