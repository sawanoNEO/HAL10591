#include "AttackObject.h"
#include "player.h"

#include "../Component/Sword.h"
#include "../Component/shader.h"
#include "../Component/Colider.h"

#include "../Scene/scene.h"

#include "../System/modelRenderer.h"
#include "../System/manager.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void AttackObject::Init()
{
	//AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\unlitTexturePS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\Player\\Sword.obj");
	//m_Scale = Vector3{ 0.5f,0.5f,0.5f };
	//m_Position = Vector3{ 1.0f,1.0f,1.0f };
	AddComponent<Colider>();
	GetComponent<Colider>()->Init(DEFAULT, Vector3{1.0f,1.0f,1.0f});
	m_Scale = Vector3{ 4.0f,2.0f,4.0f };
}

void AttackObject::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = player->GetForward();

    m_Position = player->GetPosition();

	m_Position += forward * 3;
}

void AttackObject::Draw()
{
	ImGui::Begin("AttackObj");
	ImGui::Text("%f%f%f",m_Position.x,m_Position.y,m_Position.z);
	ImGui::End();
}
