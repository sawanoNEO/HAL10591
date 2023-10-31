#include "WallDodgeCamera.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
//#include "input.h"
//#include "scene.h"
#include "modelRenderer.h"
#include "box.h"
//#include "shadow.h"
#include "shader.h"
#include "Colider.h"
#include <SimpleMath.h>
#include "Rigidbody.h"

#include "player.h"
#include "imguimanager.h"
#include "camera.h"
#include <vector>

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace std;

void WallDodgeCamera::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

	AddComponent<Colider>();
	GetComponent<Colider>()->Init(WALL, Vector3{ 0.1f, 0.1f , 0.1f });
	m_Scale.x = 0.1f;
	m_Scale.y = 0.1f;
	m_Scale.z = 0.1f;
}

void WallDodgeCamera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	vector<Box*> gameobject = scene->GetGameObjects<Box>();
}

void WallDodgeCamera::Draw()
{
	Scene* scene = Manager::GetScene();
	vector<Box*> gameobject = scene->GetGameObjects<Box>();
	ImGui::Begin("WallCamera");
	ImGui::Text("gameobject%f", gameobject[0]->GetPosition().x);
	ImGui::End();
}
