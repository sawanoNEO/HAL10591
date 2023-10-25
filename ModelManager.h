#pragma once
#include "component.h"

class AnimationModel;
class Player;

class ModelManager : public Component
{
private:
	int m_frame;
	float	m_BlendRate;
	AnimationModel* animationmodel;
	char AnimationName;
	Player* player;
public:
	using Component::Component;
	void Init();
	void Update()override;
	void Draw()override;
};

