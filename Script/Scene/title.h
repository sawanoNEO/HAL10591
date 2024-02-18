#pragma once

#include "../Scene/scene.h"

class Title : public Scene
{
private:
	enum Cursle
	{
		START,ENDLESS,EXIT,
	};
	class Transition* m_Transition{};
	class GameObject* m_gameobject;
	class GameObject* m_Arrow;
	class Audio* m_BGM{};
	int m_Cursle=0;
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};