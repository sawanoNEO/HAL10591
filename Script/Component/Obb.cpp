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

BoundingBoxOBB Obb::MakeOBB(// 対象オブジェクトのワールド変換行列
		Vector3 scale) {			// 拡縮率

			// 出力用
			BoundingBoxOBB obb;
			obb = m_obbinfo;

			Matrix worldmtx = m_GameObject->GetMatrix();

			// 平行移動
			obb.worldcenter = Vector3::Transform(m_obbinfo.center, worldmtx);


			// 移動成分カット
			worldmtx._41 = 0.0f;
			worldmtx._42 = 0.0f;
			worldmtx._43 = 0.0f;

			// 軸回転
			obb.axisX = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisX, worldmtx);
			obb.axisY = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisY, worldmtx);
			obb.axisZ = DirectX::SimpleMath::Vector3::Transform(m_obbinfo.axisZ, worldmtx);

			// 軸を正規化
			obb.axisX.Normalize();
			obb.axisY.Normalize();
			obb.axisZ.Normalize();

			// 拡縮を考慮
			obb.lengthx *= scale.x;
			obb.lengthy *= scale.y;
			obb.lengthz *= scale.z;

			return obb;
}

bool Obb::CompareLength(const BoundingBoxOBB& ObbA,		// OBB-A
	const BoundingBoxOBB& ObbB,		// OBB-B
	const Vector3& vecSeparate,		// 分離軸
	const Vector3& vecDistance)		// 中心座標を結んだベクトル
{
	float fDistance{};

	// 分離軸に射影した中心間を結ぶベクトル
	fDistance = vecDistance.Dot(vecSeparate);	// 内積を計算
	fDistance = fabsf(fDistance);				// プラスにする

	//分離軸上にボックスAを射影した影の長さ
	float fShadowA = 0;

	//分離軸上にボックスＢを射影した影の長さ
	float fShadowB = 0;

	//ボックスAの”影”を算出
	float fShadowAx{};
	float fShadowAy{};
	float fShadowAz{};

	// X軸を分離軸に射影
	fShadowAx = vecSeparate.Dot(ObbA.axisX);
	fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

	// Y軸を分離軸に射影
	fShadowAy = vecSeparate.Dot(ObbA.axisY);
	fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

	// Z軸を分離軸に射影
	fShadowAz = vecSeparate.Dot(ObbA.axisZ);
	fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

	// 分離軸に射影した影の長さを求める
	fShadowA = fShadowAx + fShadowAy + fShadowAz;

	//ボックスBの”影”を算出
	float fShadowBx{};
	float fShadowBy{};
	float fShadowBz{};

	// X軸を分離軸に射影
	fShadowBx = vecSeparate.Dot(ObbB.axisX);
	fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

	// Y軸を分離軸に射影
	fShadowBy = vecSeparate.Dot(ObbB.axisY);
	fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

	// Z軸を分離軸に射影
	fShadowBz = vecSeparate.Dot(ObbB.axisZ);
	fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

	// 分離軸に射影した影の長さを求める
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

// OBBの当たり判定を行う
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB) {

	//分離軸
	Vector3 vecSeparate;

	// 2つのオブジェクトを結んだベクトルを計算
	Vector3 vecDistance;
	vecDistance = obbB.worldcenter - obbA.worldcenter;

	bool sts;	// 戻り値

	// OBB-A軸リスト
	const Vector3* OBB_A_Axis[3] = {
			&obbA.axisX,
			&obbA.axisY,
			&obbA.axisZ
	};

	// OBB-B軸リスト
	const Vector3* OBB_B_Axis[3] = {
			&obbB.axisX,
			&obbB.axisY,
			&obbB.axisZ
	};

	// OBB-Aの３軸を分離軸にしてチェック
	for (int i = 0; i < 3; i++) {

		Vector3 vecSeparate = *OBB_A_Axis[i];

		sts = Obb::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// 分離軸
			vecDistance);		// ２つのオブジェクトを結ぶベクトル
		if (sts == false) {
			return false;
		}
	}

	// OBB-Bの３軸を分離軸にしてチェック
	for (int i = 0; i < 3; i++) {
		Vector3 vecSeparate = *OBB_B_Axis[i];

		sts = Obb::CompareLength(
			obbA,				// OBB-A
			obbB,				// OBB-B
			vecSeparate,		// 分離軸
			vecDistance);		// ２つのオブジェクトを結ぶベクトル
		if (sts == false) {
			return false;
		}
	}

	// 外積を分離軸として計算
	for (int p1 = 0; p1 < 3; p1++) {
		for (int p2 = 0; p2 < 3; p2++) {

			Vector3 obbAaxis = *OBB_A_Axis[p1];
			Vector3 obbBaxis = *OBB_B_Axis[p2];

			Vector3	crossseparate;			// 外積ベクトル
			crossseparate = obbAaxis.Cross(obbBaxis);

			sts = Obb::CompareLength(
				obbA,				// OBB-A
				obbB,				// OBB-B
				crossseparate,		// 分離軸
				vecDistance);		// ２つのオブジェクトを結ぶベクトル
			if (sts == false) {
				return false;
			}
		}
	}

	return true;
}