#pragma once
#include "scene.h"
class Endless : public Scene
{
private:
	class Transition* m_Transition{};
	bool m_Goal = false;
	GameObject* m_LoadImage;
	GameObject* m_LoadBackImage;
	bool m_BossApearance = false;//ボスが登場するためのフラグ
	int m_LoadCount = 0;
	void ChangeLoadImage();

public:

	void LoadImageDraw()override;
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()override;
};

