#pragma once
#include "enemy.h"
class Boss :  public Enemy
{
private:
	int m_DamageCount = 0;//���݂܂ł̊Ԃɉ���U�����󂯂���
	int m_ReactionCount = 3;//���x�܂łЂ�܂��ɍU���ɑς����邩
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	bool Damage(float);
	void HitReset();

	void SetAnimName2(const char*) override;//�A�j���[�V�����؂�ւ�
};

