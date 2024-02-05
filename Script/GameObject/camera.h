#pragma once

#include	<SimpleMath.h>
#include	"gameObject.h"

class Colider;//前方宣言

class Camera : public GameObject{
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};
	DirectX::SimpleMath::Vector3    m_GoalPosition{};//目標地点
	float angle;
	float Yangle;      //Y軸計算用の変数
	DirectX::SimpleMath::Vector3    camForward;
	float Distance=13.0f;//カメラとプレイヤー間の距離
	bool rock;
	GameObject* RockEnemy;
	Colider* camcol;    //カメラの壁回避のために当たり判定を付ける

	float TargetY;//ターゲット時のカメラの高さ(調整用)

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Begin();
	DirectX::SimpleMath::Vector3 GetCamForward();   //カメラの向いている方向を取得
	DirectX::SimpleMath::Vector3 GetCamSide();      //↑の横の方向を取得するver
	DirectX::SimpleMath::Vector3 VecYRemove(DirectX::SimpleMath::Vector3);      //カメラの前向きベクトルのY成分を取り除く処理(移動に使う)
	DirectX::SimpleMath::Matrix GetViewMatrix() 
	{ 
		return m_ViewMatrix;
	}
	bool GetRock() { return rock; }
	GameObject* GetRockEnemy() { return RockEnemy; }
};