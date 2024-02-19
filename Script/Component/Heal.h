#pragma once
#include "Item.h"
class Heal : public Item
{
private:
	int m_ParticleNum=1;
public:
	void Enter()override;
	void Exit()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};

