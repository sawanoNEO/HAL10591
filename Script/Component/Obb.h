#pragma once
#include "component.h"
#include <SimpleMath.h>


// 境界ボックスOBB
struct BoundingBoxOBB{
	DirectX::SimpleMath::Vector3 max;
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// 中心軸（X軸、Y軸、Z軸）
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOXサイズ
	float	lengthx;
	float	lengthy;
	float	lengthz;
};


class Obb : public Component
{
	BoundingBoxOBB	m_obbinfo;			// OBB情報
	class ColiderLooker* col = nullptr;

public:
	using Component::Component;
	
	void Update()override;

	// ローカル座標で生成したOBBをワールド空間に変換
	//BoundingBoxOBB MakeOBB(DirectX::SimpleMath::Vector3 scale);			// 拡縮率

	//ローカル座標で生成したOBBをワールド空間に変換
	BoundingBoxOBB MakeOBB(
		// 対象オブジェクトのワールド変換行列
		DirectX::SimpleMath::Vector3 scale);			// 拡縮率

	// 分離軸に射影した影の重なりをチェックする
	static bool CompareLength(
		const BoundingBoxOBB& ObbA,								// OBB-A
		const BoundingBoxOBB& ObbB,								// OBB-B
		const DirectX::SimpleMath::Vector3& vecSeparate,		// 分離軸
		const DirectX::SimpleMath::Vector3& vecDistance);		// 中心座標を結んだベクトル

	// 中心座標を戻す
	DirectX::SimpleMath::Vector3 GetCenter();

	// BBOX描画用のワールド変換行列を生成する
	DirectX::SimpleMath::Matrix MakeWorldMtx(
		DirectX::SimpleMath::Vector3 scale,
		DirectX::SimpleMath::Matrix wmtx);

	// 最大値取得
	DirectX::SimpleMath::Vector3 GetMax();

	// 最小値取得
	DirectX::SimpleMath::Vector3 GetMin();

	// OBB取得
	BoundingBoxOBB GetOBB();
};

// OBBの当たり判定
bool HitCheckOBB(
	const BoundingBoxOBB& obbA,
	const BoundingBoxOBB& obbB);