#pragma once
#include "gameObject.h"
class AttackObject : public GameObject
{
private:
	class AnimationModel* m_Model;

public:
	void Init() override;
	void Update() override;
	void Draw()override;
};
