#pragma once
#include "../Component/component.h"

class Jump : public Component
{
private :
	int time = 0;//放物線運動の計算に使う
	float g = 1.0;//重力
	float v = 0.5f;//初速度
	float dec = 0.5;//減衰
	int i;
public:
	using Component::Component;

	void Jumping();
	void Init();
	void Update();
	void Draw();
};
