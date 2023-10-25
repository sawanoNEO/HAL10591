#pragma once

#include "gameObject.h"

class Rigidbody;

class Enemy : public GameObject
{
private:
	enum State
	{
		NONE,
		NORMAL,    //通常の状態。偵察状態
		BATTLE,    //プレイヤーを見つけて攻撃してくる状態
	};
	State state;
	float MaxHP;    //最大体力 
	float HP;       //現在体力
	bool hit;       //一回の攻撃で複数回攻撃が当たった判定にならないためのフラグ
	float groundHeight; //床の高さ
	Rigidbody* rb;
	bool Phit=false;
	float eyesight;//どのぐらいの遠さまで見えるか
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Damage(float);
	void HitReset();

	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//視野範囲チェック関数。
};