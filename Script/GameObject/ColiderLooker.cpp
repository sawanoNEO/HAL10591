#include "ColiderLooker.h"
#include "../Component/shader.h"
#include "../Component/Colider.h"

#include "../System/manager.h"
#include "../System/modelRenderer.h"

#include "../Scene/scene.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void ColiderLooker::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\Colider1.obj");

}

void ColiderLooker::Update()
{
}

void ColiderLooker::Draw()
{
#if _DEBUG
	ImGui::Begin("coliderlook");
	ImGui::Text("%f,%f,%f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("%f,%f,%f", m_Scale.x, m_Scale.y, m_Scale.z);
	ImGui::End();
#endif
}