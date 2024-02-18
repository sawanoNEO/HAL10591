#pragma once
#include "enemy.h"
class Boss :  public Enemy
{
private:
	int m_DamageCount = 0;//怯みまでの間に何回攻撃を受けたか
	int m_ReactionCount = 3;//何度までひるまずに攻撃に耐えられるか
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	bool Damage(float);
	void HitReset();

	void SetAnimName2(const char*) override;//アニメーション切り替え
};

