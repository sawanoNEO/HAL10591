#pragma once
#include "Item.h"
class Heal : public Item
{
public:
	void Enter()override;
	void Exit()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};

