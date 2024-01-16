#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
//#include "../System/input.h"
//#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
#include "../GameObject/cylinder.h"
//#include "shadow.h"
#include "../Component/shader.h"



void Cylinder::Init()
{

	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\cylinder.obj");


//	AddComponent<Shadow>()->SetSize(5.0f);

}
