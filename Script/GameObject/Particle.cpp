#include "../GameObject/Particle.h"
#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../Scene/scene.h"
#include "../System/modelRenderer.h"
//#include "../GameObject/enemy.h"
#include "../GameObject/player.h"

#include "../Component/Rigidbody.h"
#include "../Component/shader.h"
#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../Component/sprite.h"

#include "../ImGui/imguimanager.h"
#include <vector>

#include <stdlib.h>
#include <time.h>

using namespace DirectX::SimpleMath;
int Particle::Num = 0;
void Particle::Init()
{
	
	//AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<ModelRenderer>()->Load("asset\\model\\enemy.obj");
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	switch (rand() % 3)
	{
	case 0:
		AddComponent<Sprite>()->Init(50.0f, 700.0f, 64 , 64 , "asset\\texture\\titleFlashR.png");
		break;
	case 1:
		AddComponent<Sprite>()->Init(50.0f, 700.0f, 64 , 64 , "asset\\texture\\titleFlashB.png");
		break;
	case 2:
		AddComponent<Sprite>()->Init(50.0f, 700.0f, 64 , 64 , "asset\\texture\\titleFlashY.png");
		break;
	}
	m_Position = Vector3(50.0f+(rand()%3*500), 700.0f, 0.f);
	v = (rand()%500);
	v /= 100;
	while (v == 0)
	{
		v = (rand() % 500);
		v /= 100;
	}
	angle = rand() % 360;
	g = 5.1;
	Num++;
}

void Particle::Update()
{
		float realv = abs(v);
		T++;
		//cnt++;
		m_Position.x += T / 60 *( v * cos(angle * 3.14 / 180));
		m_Position.y += T / 60 * realv * (sin(angle * 3.14 / 180) - (g * ((T / 60) * (T / 60))) / 2);
		GetComponent<Sprite>()->SetVertex(m_Position.x, m_Position.y, 64, 64);
	if (m_Position.y<-1.0)
	{
		Num--;
		SetDestroy();
	}
}

void Particle::Draw()
{
	ImGui::Begin("Particle");
	//ImGui::Text("pos%f%f%f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("%f", v);
	ImGui::Text("%f", m_Position.x);
	ImGui::Text("%f", m_Position.y);
	ImGui::End();

}
