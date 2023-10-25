#include "main.h"
#include "manager.h"

#include "input.h"

#include "title.h"

#include "game.h"

#include "shader.h"
#include "sprite.h"

#include "transition.h"

using namespace DirectX;

void Title::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
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

	// ��ʑJ�ڂ��I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Game>();
	}
}
