#pragma once
#include "enemy.h"
class Boss :  public Enemy
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Damage(float);
	void HitReset();

	void SetAnimName2(const char*) override;//アニメーション切り替え
};

