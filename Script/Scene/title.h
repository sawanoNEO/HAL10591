#pragma once

#include "../Scene/scene.h"

class Title : public Scene
{
private:
	class Transition* m_Transition{};
	class GameObject* m_gameobject;
	class GameObject* m_Arrow;
	class Audio* m_BGM{};
	int m_Cursle=0;
	int m_CursleMAX = 2;
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};