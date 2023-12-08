#include "Obb.h"
#include "../GameObject/gameObject.h"
#include "../GameObject/ColiderLooker.h"
#include "../Scene/scene.h"
#include "../System/manager.h"
using namespace DirectX::SimpleMath;

void Obb::Update()
{
	Scene* scene = Manager::GetScene();

	m_obbinfo = MakeOBB(m_GameObject->GetScale());
	if (col == nullptr)
	{
		col = scene->AddGameObject<ColiderLooker>(1);
	}
	col->SetPosition(m_GameObject->GetPosition());
	col->SetScale(m_GameObject->GetScale());
	col->SetRotation(m_GameObject->GetRotation());
}

BoundingBoxOBB Obb::MakeOBB(// �ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s��
		Vector3 scale) {			// �g�k��

			// �o�͗p
			BoundingBoxOBB obb;
			obb = m_obbinfo;

			Matrix worldmtx = m_GameObject->GetMatrix();

			// ���s�ړ�
			obb.worldcenter = Vector3::Transform(m_obbinfo.center, worldmtx);


			// �ړ������J�b�g
			worldmtx._41 = 0.0f;
			worldmtx._42 = 0.0f;
			worldmtx._43 = 0.0f;

			// ����]
			obb.axisX = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisX, worldmtx);
			obb.axisY = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisY, worldmtx);
			obb.axisZ = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisZ, worldmtx);

			// ���𐳋K��
			obb.axisX.Normalize();
			obb.axisY.Normalize();
			obb.axisZ.Normalize();

			// �g�k���l��
			obb.lengthx *= scale.x;
			obb.lengthy *= scale.y;
			obb.lengthz *= scale.z;

			return obb;
}

bool Obb::CompareLength(const BoundingBoxOBB& ObbA,		// OBB-A
	const BoundingBoxOBB& ObbB,		// OBB-B
	const Vector3& vecSeparate,		// ������
	const Vector3& vecDistance)		// ���S���W�����񂾃x�N�g��
{
	float fDistance{};

	// �������Ɏˉe�������S�Ԃ����ԃx�N�g��
	fDistance = vecDistance.Dot(vecSeparate);	// ���ς��v�Z
	fDistance = fabsf(fDistance);				// �v���X�ɂ���

	//��������Ƀ{�b�N�XA���ˉe�����e�̒���
	float fShadowA = 0;

	//��������Ƀ{�b�N�X�a���ˉe�����e�̒���
	float fShadowB = 0;

	//�{�b�N�XA�́h�e�h���Z�o
	float fShadowAx{};
	float fShadowAy{};
	float fShadowAz{};

	// X���𕪗����Ɏˉe
	fShadowAx = vecSeparate.Dot(ObbA.axisX);
	fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowAy = vecSeparate.Dot(ObbA.axisY);
	fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowAz = vecSeparate.Dot(ObbA.axisZ);
	fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowA = fShadowAx + fShadowAy + fShadowAz;

	//�{�b�N�XB�́h�e�h���Z�o
	float fShadowBx{};
	float fShadowBy{};
	float fShadowBz{};

	// X���𕪗����Ɏˉe
	fShadowBx = vecSeparate.Dot(ObbB.axisX);
	fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

	// Y���𕪗����Ɏˉe
	fShadowBy = vecSeparate.Dot(ObbB.axisY);
	fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

	// Z���𕪗����Ɏˉe
	fShadowBz = vecSeparate.Dot(ObbB.axisZ);
	fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

	// �������Ɏˉe�����e�̒��������߂�
	fShadowB = fShadowBx + fShadowBy + fShadowBz;

	if (fDistance > fShadowA + fShadowB) {
		return false;
	}

	return true;
}

BoundingBoxOBB Obb::GetOBB()
{
	return m_obbinfo;
}

// OBB�̓����蔻����s��
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB) {

	//������
	Vector3 vecSeparate;

	// 2�̃I�u�W�F�N�g�����񂾃x�N�g�����v�Z
	Vector3 vecDistance;
	vecDistance = obbB.worldcenter - obbA.worldcenter;

	bool sts;	// �߂�l

	// OBB-A�����X�g
	const Vector3* OBB_A_Axis[3] = {
			&obbA.axisX,
			&obbA.axisY,
			&obbA.axisZ
	};

	// OBB-B�����X�g
	const Vector3* OBB_B_Axis[3] = {
			&obbB.axisX,
			&obbB.axisY,
			&obbB.axisZ
	};

	// OBB-A�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {

		Vector3 vecSeparate = *OBB_A_Axis[i];

		sts = Obb::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// OBB-B�̂R���𕪗����ɂ��ă`�F�b�N
	for (int i = 0; i < 3; i++) {
		Vector3 vecSeparate = *OBB_B_Axis[i];

		sts = Obb::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// ������
			vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
		if (sts == false) {
			return false;
		}
	}

	// �O�ς𕪗����Ƃ��Čv�Z
	for (int p1 = 0; p1 < 3; p1++) {
		for (int p2 = 0; p2 < 3; p2++) {

			Vector3 obbAaxis = *OBB_A_Axis[p1];
			Vector3 obbBaxis = *OBB_B_Axis[p2];

			Vector3	crossseparate;			// �O�σx�N�g��
			crossseparate = obbAaxis.Cross(obbBaxis);

			sts = Obb::CompareLength(
				obbA,				// OBB-A
				obbB,				// OBB-B
				crossseparate,		// ������
				vecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��
			if (sts == false) {
				return false;
			}
		}
	}

	return true;
}