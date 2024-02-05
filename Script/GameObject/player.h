#pragma once
#include "gameObject.h"
#include <iostream>
#include <unordered_map>

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
	std::unordered_map< const char*,class Audio*> m_SE{};

	class AnimationModel* m_Model;
	AnimationModel* m_ChildBone;
	float m_Frame1;//再生中のアニメーションのフレーム数
	float m_Frame2;//再生中のアニメーションのフレーム数
	float m_BlendRate;

	float MaxHP=1000;     //最大体力
	float HP=1000;             //現在体力
	float groundHeight; //床の高さ
	float ST=100;           //スタミナ
	float MaxST = 100;        //スタミナ最大値
	float Speed;        //基本の移動速度
	float Accel;        //通常の加速度
	float DAccel;       //ダッシュ時の加速度
	float MaxSpeed;     //最大移動速度
	float cnt;          //各動作のカウント
	float m_AnimSpeed = 1.0f;//アニメーションの再生速度(倍率)
	float m_RecoverST = 0.75f;//スタミナの回復速度(1fごとの回復量)

	bool Action=false;  //スタミナを消費する行動をしているか
	bool Wait = false;  //ダッシュでスタミナを使い切った時にtrueになる。スタミナが回復しきるまでダッシュ出来ないという判定に用いる
	bool m_Invincible = false; //回避などの無敵中かどうか

	int m_InvincibleFrame = 0;//無敵時間が持続するフレーム

	DirectX::SimpleMath::Vector3 oldPosition;
	Colider* colme;     //自分の当たり判定
	Colider* colattack;  //攻撃時の当たり判定
	DirectX::SimpleMath::Vector3 promissDirection{ 0.0f,0.0f,1.0f };//プレイヤーが振り向く際の最終的に振り向く方向
	std::string m_Animname1="Idle"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(1)
	std::string m_Animname2="Walk"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(2)

	class AttackObject* m_Child = {};

public:

	float alpha;//プレイヤーのアルファ値
	void Init() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;

	void Damage(float _damage)override;
	void STRecover();       //スタミナを回復する関数
	void STUse(float);           //スタミナを消費する関数
	void ASHP(float hp);//HPを増減させる関数

	//以下は各変数ごとのセッター&ゲッター
	 
	//現在のプレイヤーの状態を返す
	PLAYERSTATE GetPstate() { return Pstate;}
	float GetST() { return ST; }
	bool GetWait() { return Wait; }
	Colider* Getcolattack() { return colattack; }
	float GetMaxST() { return MaxST; }
	float GetMaxHP() { return MaxHP; }
	float GetHP() { return HP; }
	bool GetInvincible() { return m_Invincible; }
	DirectX::SimpleMath::Vector3 GetpromissDirection() { return promissDirection; }
	DirectX::SimpleMath::Vector3 GetoldPosition() { return oldPosition; }

	void SetPstate(PLAYERSTATE s) { Pstate = s; }	//プレイヤーの状態を変える。引数には変えたい状態を入れる
	void SetVerocity(float);
	void SetWait(bool wait) { Wait = wait; }
	void SetoldPosition(DirectX::SimpleMath::Vector3 pos) { oldPosition = pos; }
	void SetpromissDirection(DirectX::SimpleMath::Vector3 _dir) { promissDirection = _dir; }
	void SetInvincible(bool _invincible) { m_Invincible = _invincible; }//無敵状態にする(1fだけ,繰り返して使う前提)
	void SetInvincibleFrame(int _frame) { m_InvincibleFrame = _frame; }//無敵時間を設定する(フレーム)
	void SetAnimName2(const char*);
	void SetAnimSpeed(float _speed)override { m_AnimSpeed = _speed; }//アニメーションの再生速度の設定
	void SetFrame1(int _frame) { m_Frame1 = _frame; }
	void SetFrame2(int _frame) { m_Frame2 = _frame; }
	std::unordered_map<const char*, Audio*> GetSEs() { return m_SE; }
	void PlaySE(const char* _SEname)override;
	void PlaySE(const char* _SEname,bool _loop)override;
	void StopSE(const char* _SEname)override;
	//DirectX::SimpleMath::Vector3 GetOldPos() { return oldPosition; }
};