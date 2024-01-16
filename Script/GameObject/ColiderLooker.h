#pragma once
#include "../GameObject/gameObject.h"


class ColiderLooker : public GameObject
{

private:
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};