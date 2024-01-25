#pragma once
#include "gameObject.h"
class AttackObject : public GameObject
{
private:
	class AnimationModel* m_Model;
	int flga = 1;
	int flgb = 1;
	int flgc = 1;
public:
	void Init() override;
	void Update() override;
	void Draw()override;



	//Mattrixhenkan(const class aiMatrix4x4 aiMat);
};
