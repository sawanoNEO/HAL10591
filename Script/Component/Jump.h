#pragma once
#include "../Component/component.h"

class Jump : public Component
{
private :
	int time = 0;//�������^���̌v�Z�Ɏg��
	float g = 1.0;//�d��
	float v = 0.5f;//�����x
	float dec = 0.5;//����
	int i;
public:
	using Component::Component;

	void Jumping();
	void Init();
	void Update();
	void Draw();
};
