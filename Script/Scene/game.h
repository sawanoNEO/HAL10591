#pragma once

#include "../Scene/scene.h"

class Game : public Scene
{
private:
	class Transition*	m_Transition{};
	class Audio*		m_BGM{};
	bool m_Goal = false;
	GameObject* m_LoadImage;
	GameObject* m_LoadBackImage;

	int m_LoadCount = 0;
	void ChangeLoadImage();
public:

	void LoadImageDraw()override;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;

};