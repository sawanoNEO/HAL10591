#pragma once
#include	<cinttypes>
#include	"../Scene/scene.h"

// ëOï˚éQè∆
class Application;

class Manager
{
private:
	static class Scene* m_Scene;
	static bool m_Exit;

public:
	static void Init(Application* ap);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	static class Scene* GetScene()
	{
		return m_Scene;
	}

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->UninitBase();
			delete m_Scene;
		}
		m_Scene = new T();
		m_Scene->LoadImageDraw();
		m_Scene->InitBase();
	}

	static void GameExit() { m_Exit = true; }
	static bool GetExit() { return m_Exit; }
};