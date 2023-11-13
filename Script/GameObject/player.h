#pragma once
#include "gameObject.h"
#include <iostream>

enum PLAYERSTATE
{
	NONE,
	DASH,
	ATTACK,
	ROLLING,
	JUMP,
	ITEM,
	BACKSTEP,
	DEATH,
	};

	class Colider;

class Player : public GameObject
{
private:
	PLAYERSTATE Pstate = NONE;
	DirectX::SimpleMath::Vector3		m_Velocity{};
	class Audio*	m_SE{};

	class AnimationModel* m_Model;
	float m_Frame1;//再生中のアニメーションのフレーム数
	float m_Frame2;//再生中のアニメーションのフレーム数
	float m_BlendRate;

	float MaxHP=1000;     //最大体力
	float HP;             //現在体力
	float groundHeight; //床の高さ
	float ST;           //スタミナ
	float MaxST = 100;        //スタミナ最大値
	float Speed;        //基本の移動速度
	float Accel;        //通常の加速度
	float DAccel;       //ダッシュ時の加速度
	float MaxSpeed;     //最大移動速度
	float cnt;          //各動作のカウント
	bool Action=false;  //スタミナを消費する行動をしているか
	bool Wait = false;  //ダッシュでスタミナを使い切った時にtrueになる
	DirectX::SimpleMath::Vector3 oldPosition;
	Colider* colme;     //自分の当たり判定
	Colider* colattack;  //攻撃時の当たり判定
	DirectX::SimpleMath::Vector3 promissDirection{ 0.0f,0.0f,1.0f };//プレイヤーが振り向く際の最終的に振り向く方向
	std::string m_Animname1="Idle"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(1)
	std::string m_Animname2="Walk"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(2)

public:

	float alpha;//プレイヤーのアルファ値
	void Init() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;

	void STRecover(float);       //スタミナを回復する関数
	void STUse(float);           //スタミナを消費する関数

	//以下は各変数ごとのセッター&ゲッター
	 
	//現在のプレイヤーの状態を返す
	PLAYERSTATE GetPstate() { return Pstate;}
	void SetPstate(PLAYERSTATE s) { Pstate = s; }	//プレイヤーの状態を変える。引数には変えたい状態を入れる
	float GetST() { return ST; }
	void SetVerocity(float);
	bool GetWait() { return Wait; }
	void SetWait(bool wait) { Wait = wait; }
	Colider* Getcolattack() { return colattack; }
	float GetMaxST() { return MaxST; }

	float GetMaxHP() { return MaxHP; }
	float GetHP() { return HP; }
	void ASHP(float hp);

	DirectX::SimpleMath::Vector3 GetpromissDirection() { return promissDirection; }
	void SetpromissDirection(DirectX::SimpleMath::Vector3 _dir) { promissDirection = _dir; }

	void SetAnimName2(const char*);

	//DirectX::SimpleMath::Vector3 GetOldPos() { return oldPosition; }
};