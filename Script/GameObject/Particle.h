#pragma once
#include "../GameObject/gameObject.h"
#include <SimpleMath.h>
#include <vector>
//#include "../GameObject/enemy.h"


class Particle :  public GameObject
{
private: 
	static int Num;

	float v;//�����x
	float vx;//�����xx����
	float vy;
	float T;//flame
	float g;//�d��
	int angle;//��
	int cnt=0;
public:
	void Init() override;
	void Update() override;
	void Draw()override;
	static int GetNum() { return Num; }
};

