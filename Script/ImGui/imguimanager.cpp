#include "../ImGui/imguimanager.h"
#include "../System/Application.h"
#include"../System/renderer.h"

Application* ImGuiManager::m_Application;

void ImGuiManager::ImguiInit(Application* ap)
{
	m_Application = ap;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("asset\\font\\meiryo.ttc", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	ImGui_ImplWin32_Init(m_Application->GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(),Renderer::GetDeviceContext());
	ImGui::StyleColorsLight();
}

void ImGuiManager::ImguiUnInit()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
