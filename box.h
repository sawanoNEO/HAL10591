#pragma once

#include "gameObject.h"

class Box : public GameObject
{

private:

public:
	void Init() override;
	void Update() override;
	void Draw()override;
};