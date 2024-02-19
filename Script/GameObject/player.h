#pragma once
#include "gameObject.h"
#include <iostream>
#include <unordered_map>

	class Colider;

class Player : public GameObject
{
private:
	DirectX::SimpleMath::Vector3		m_Velocity{};
	std::unordered_map< const char*,class Audio*> m_SE{};

	class AnimationModel* m_Model;

	float m_Frame1;//再生中のアニメーションのフレーム数
	float m_Frame2;//再生中のアニメーションのフレーム数
	float m_BlendRate;

	float m_MaxHP=1000;     //最大体力
	float m_HP=1000;             //現在体力
	float m_GroundHeight; //床の高さ
	float m_ST=100;           //スタミナ
	float m_MaxST = 100;        //スタミナ最大値
	float m_Speed;        //基本の移動速度
	float m_Accel;        //通常の加速度
	float m_DAccel;       //ダッシュ時の加速度
	float m_MaxSpeed;     //最大移動速度
	float m_AnimSpeed = 1.0f;//アニメーションの再生速度(倍率)
	float m_RecoverST = 0.75f;//スタミナの回復速度(1fごとの回復量)

	bool m_Wait = false;  //ダッシュでスタミナを使い切った時にtrueになる。スタミナが回復しきるまでダッシュ出来ないという判定に用いる
	bool m_Invincible = false; //回避などの無敵中かどうか

	int m_InvincibleFrame = 0;//無敵時間が持続するフレーム

	DirectX::SimpleMath::Vector3 m_OldPosition;
	Colider* m_Colme;     //自分の当たり判定
	DirectX::SimpleMath::Vector3 m_PromissDirection{ 0.0f,0.0f,1.0f };//プレイヤーが振り向く際の最終的に振り向く方向
	std::string m_Animname1="Idle"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(1)
	std::string m_Animname2="Walk"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(2)

	class AttackObject* m_Child = {};

public:

	float alpha;//プレイヤーのアルファ値
	void Init() override;
	void Update() override;
	void Draw() override;

	bool Damage(float _damage)override;
	void STRecover();       //スタミナを回復する関数
	void STUse(float);           //スタミナを消費する関数
	void ASHP(float hp);//HPを増減させる関数

	//以下は各変数ごとのセッター&ゲッター
	 
	//現在のプレイヤーの状態を返す
	float GetST() { return m_ST; }
	bool GetWait() { return m_Wait; }
	float GetMaxST() { return m_MaxST; }
	float GetMaxHP() { return m_MaxHP; }
	float GetHP() { return m_HP; }
	bool GetInvincible() { return m_Invincible; }
	DirectX::SimpleMath::Vector3 GetpromissDirection() { return m_PromissDirection; }
	DirectX::SimpleMath::Vector3 GetoldPosition() { return m_OldPosition; }

	void SetVerocity(float);
	void SetWait(bool wait) { m_Wait = wait; }
	void SetoldPosition(DirectX::SimpleMath::Vector3 pos) { m_OldPosition = pos; }
	void SetpromissDirection(DirectX::SimpleMath::Vector3 _dir) { m_PromissDirection = _dir; }
	void SetInvincible(bool _invincible);//無敵状態にする(1fだけ,繰り返して使う前提)
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