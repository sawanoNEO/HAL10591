#pragma once

#include "../Scene/scene.h"

class Title : public Scene
{
private:
	class Transition* m_Transition{};
	class GameObject* m_gameobject;

public:
	void Init() override;
	void Update() override;
	void Draw() override;
};