#pragma once
#include "gameObject.h"
class WallDodgeCamera : public GameObject
{
private:
public: 
	void Init() override;
	void Update() override;
	void Draw()override;
};

