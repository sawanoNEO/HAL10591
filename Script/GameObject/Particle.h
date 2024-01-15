#pragma once
#include "../GameObject/gameObject.h"
#include <SimpleMath.h>
#include <vector>
//#include "../GameObject/enemy.h"


class Particle :  public GameObject
{
private: 
	static int Num;

	float v;//初速度
	float vx;//初速度x方向
	float vy;
	float T;//flame
	float g;//重力
	int angle;//θ
	int cnt=0;
public:
	void Init() override;
	void Update() override;
	void Draw()override;
	static int GetNum() { return Num; }
};

