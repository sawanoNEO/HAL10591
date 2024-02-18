//#include <WICTextureLoader.h>
#include "../Component/sprite.h"
#include "../Component/shader.h"
#include "../GameObject/Polygon2D.h"
//#include "../System/renderer.h"

using namespace DirectX::SimpleMath;

void Polygon2D::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(0, 0, 200, 200, "asset\\texture\\grass.jpg");
}


void Polygon2D::Uninit()
{

	for (auto& cmpt : m_Component) {
		cmpt->Uninit();
		delete cmpt;
	}
	m_Component.clear();
}

void Polygon2D::Update()
{
}

void Polygon2D::Draw()
{

	for (auto& cmpt : m_Component) {
		cmpt->Draw();
	}
}