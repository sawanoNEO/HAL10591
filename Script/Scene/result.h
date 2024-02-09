#pragma once

#include "../Scene/scene.h"

class Result : public Scene
{
private:
	class Transition* m_Transition{};
	bool m_Winflg = false;//ゲームクリアしたかどうか

public:
	void Init() override;
	void Update() override;
	void SetResult(bool _bool) { m_Winflg = _bool; }
};