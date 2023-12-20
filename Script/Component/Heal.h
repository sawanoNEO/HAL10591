#pragma once
#include "Item.h"
class Heal : public Item
{
public:
	using Item::Item;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};

