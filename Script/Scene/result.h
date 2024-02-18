#pragma once

#include "../Scene/scene.h"

class Result : public Scene
{
private:
	class Transition* m_Transition{};
	static bool m_Winflg;//ÉQÅ[ÉÄÉNÉäÉAÇµÇΩÇ©Ç«Ç§Ç©
	int m_Cursle = 0;
	class GameObject* m_Arrow;
	class Audio* m_BGM;
	enum Cursle
	{
		RETRY,TITLE,EXIT
	};
public:
	void Init() override;
	void Update() override;
	static void SetResult(bool _bool) { m_Winflg = _bool; }
};