#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
//#include "../System/input.h"
#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "../GameObject/sky.h"
#include "../GameObject/camera.h"
#include "../Component/shader.h"


using namespace DirectX::SimpleMath;

void Sky::Init()
{
	m_Scale = Vector3(100.0f, 100.0f, 100.0f);


	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");

}


void Sky::Update()
{

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	Vector3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;

}


