#pragma once

#include "../Scene/scene.h"

class Title : public Scene
{
private:
	class Transition* m_Transition{};


public:
	void Init() override;
	void Update() override;

};