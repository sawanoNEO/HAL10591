#pragma once

#include "../GameObject/gameObject.h"
#include <string>
#include <unordered_map>

class Rigidbody;

class Enemy : public GameObject
{
protected:
	static std::vector<Enemy*> m_Enemyes;//現在のシーンで何体の敵が存在しているか
	const char* m_FileDataPath;//パラメータ情報が入っているcsvのパス
	int m_Number=0;//インスタンス毎に振り分けられているIDのようなもの、解放処理で利用する
	class AnimationModel* m_Model;
	std::unordered_map< const char*, class Audio*> m_SE{};
	float m_Frame1;//再生中のアニメーションのフレーム数
	float m_Frame2;//再生中のアニメーションのフレーム数
	float m_BlendRate;

	float MaxHP;    //最大体力 
	float HP;       //現在体力
	bool hit;       //一回の攻撃で複数回攻撃が当たった判定にならないためのフラグ
	float groundHeight; //床の高さ
	Rigidbody* rb;
	bool Phit = false;
	float m_EyeSight=15.0f;//どのぐらいの遠さまで見えるか

	std::string m_Animname1 = "Idle"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(1)
	std::string m_Animname2 = "Walk"; //アニメーションの再生時のアニメーション指定を動的に行うための変数(2)
	float m_AnimSpeed = 1.0f;//アニメーションの再生速度(倍率)

	class EnemyHP* m_HP;

public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit()override;
	bool Damage(float);
	void HitReset();

	void SetAnimName2(const char*) override;//アニメーション切り替え
	void SetAnimSpeed(float _speed)override { m_AnimSpeed = _speed; }//アニメーションの再生速度の設定

	float GetEyeSight() { return m_EyeSight; }
	static std::vector<Enemy*> GetEnemys();

	void PlaySE(const char* _SEname)override;
	void PlaySE(const char* _SEname, bool _loop)override;
	void StopSE(const char* _SEname)override;
};