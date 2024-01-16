#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class Application;

class ImGuiManager
{
private:
	static Application* m_Application;

public:
	static void ImguiInit(Application* ap);
	static void ImguiUnInit();
	static void Begin();
	static void End();
};

