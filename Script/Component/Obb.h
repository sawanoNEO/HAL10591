#pragma once
#include "component.h"
#include <SimpleMath.h>


// ���E�{�b�N�XOBB
struct BoundingBoxOBB{
	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// ���S���iX���AY���AZ���j
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOX�T�C�Y
	float	lengthx;
	float	lengthy;
	float	lengthz;
};


class Obb : public Component
{
	BoundingBoxOBB	m_obbinfo;			// OBB���
	class ColiderLooker* col = nullptr;

public:
	using Component::Component;
	
	void Update()override;

	// ���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
	//BoundingBoxOBB MakeOBB(DirectX::SimpleMath::Vector3 scale);			// �g�k��

	//���[�J�����W�Ő�������OBB�����[���h��Ԃɕϊ�
	BoundingBoxOBB MakeOBB(
		// �ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s��
		DirectX::SimpleMath::Vector3 scale);			// �g�k��

	// �������Ɏˉe�����e�̏d�Ȃ���`�F�b�N����
	static bool CompareLength(
		const BoundingBoxOBB& ObbA,								// OBB-A
		const BoundingBoxOBB& ObbB,								// OBB-B
		const DirectX::SimpleMath::Vector3& vecSeparate,		// ������
		const DirectX::SimpleMath::Vector3& vecDistance);		// ���S���W�����񂾃x�N�g��

	// ���S���W��߂�
	DirectX::SimpleMath::Vector3 GetCenter();

	// BBOX�`��p�̃��[���h�ϊ��s��𐶐�����
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	// �ő�l�擾
	DirectX::SimpleMath::Vector3 GetMax();

	// �ŏ��l�擾
	DirectX::SimpleMath::Vector3 GetMin();

	// OBB�擾
	BoundingBoxOBB GetOBB();
};

// OBB�̓����蔻��
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB);