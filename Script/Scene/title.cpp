#include "../System/main.h"
#include "../System/manager.h"

#include "../System/input.h"

#include "../Scene/title.h"

#include "../Scene/game.h"

#include "../Component/shader.h"
#include "../Component/../Component/sprite.h"

#include "../GameObject/transition.h"
#include "../GameObject/Particle.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

int x = 0;

void Title::Init()
{
	m_gameobject = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	m_gameobject->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_gameobject->AddComponent<Sprite>()->Init(500.0f, 1080.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\TitleBack.jpg");
	m_gameobject->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, 510*2,112*2, "asset\\texture\\Title.png");

	for (int i = 0; i < 100; i++)
	{
		AddGameObject<Particle>(3);
	}

	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();
}


void Title::Update()
{

	if (m_Transition->GetState() == Transition::State::Stop)
	{
		if (Input::GetController(Input::b,Input::PRESSED))
		{
			m_Transition->FadeOut();
		}
	}

	if (Particle::GetNum() < 100)
	{
		GameObject* object = AddGameObject<Particle>(3);
	}

	m_gameobject->GetComponent<Sprite>()->SetVertex(x, x, SCREEN_WIDTH, SCREEN_HEIGHT);
	//x += 1;
	// 画面遷移が終了してるか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}

void Title::Draw()
{
	ImGui::Begin("www");
	ImGui::Text("%f%f%f", m_gameobject->GetPosition().x, m_gameobject->GetPosition().y, m_gameobject->GetPosition().z);
	ImGui::Text("%i", x);
	ImGui::End();
}
