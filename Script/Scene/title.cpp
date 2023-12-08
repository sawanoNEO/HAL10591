#include "../System/main.h"
#include "../System/manager.h"

#include "../System/input.h"

#include "../Scene/title.h"

#include "../Scene/game.h"

#include "../Component/shader.h"
#include "../Component/../Component/sprite.h"

#include "../GameObject/transition.h"

using namespace DirectX;

void Title::Init()
{
	GameObject* titleBack = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleBack->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleBack->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\TitleBack.jpg");
	
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, 510*2,112*2, "asset\\texture\\Title.png");

	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();
}


void Title::Update()
{

	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetController(Input::b,Input::PRESSED))
		{
			m_Transition->FadeOut();
		}
	}

	// 画面遷移が終了してるか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}
