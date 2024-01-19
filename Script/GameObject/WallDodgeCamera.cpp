#include "../GameObject/WallDodgeCamera.h"
#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
//#include "../System/input.h"
//#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "../GameObject/box.h"
//#include "shadow.h"
#include "../Component/shader.h"
#include "../Component/Colider.h"
#include <SimpleMath.h>
#include "../Component/Rigidbody.h"

#include "../GameObject/player.h"
#include "../ImGui/imguimanager.h"
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
	//Scene* scene = Manager::GetScene();
	//vector<Box*> gameobject = scene->GetGameObjects<Box>();
	//ImGui::Begin("WallCamera");
	//ImGui::Text("gameobject%f", gameobject[0]->GetPosition().x);
	//ImGui::End();
}
