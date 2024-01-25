#pragma once

#include "../GameObject/gameObject.h"

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
	class AnimationModel* m_Model;
	float m_Frame1;//再生中のアニメーションのフレーム数
	float m_Frame2;//再生中のアニメーションのフレーム数
	float m_BlendRate;

	float MaxHP;    //最大体力 
	float HP;       //現在体力
	bool hit;       //一回の攻撃で複数回攻撃が当たった判定にならないためのフラグ
	float groundHeight; //床の高さ
	Rigidbody* rb;
	bool Phit = false;
	float eyesight;//どのぐらいの遠さまで見えるか

	std::string m_Animname1 = "Idle"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(1)
	std::string m_Animname2 = "Walk"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(2)

	class EnemyHP* m_HP;



public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Damage(float);
	void HitReset();

	void SetAnimName2(const char*);//アニメーション切り替え

	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//視野範囲チェック関数。
};