#include "../System/main.h"
#include "../System/manager.h"

#include "../System/input.h"

#include "../Scene/title.h"

#include "../Scene/game.h"

#include "../Component/shader.h"
#include "../Component/sprite.h"

#include "../GameObject/transition.h"

#include "result.h"

using namespace DirectX;

void Result::Init()
{
	GameObject* resultLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	resultLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

	if (m_Winflg == true)
	{
		resultLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\BackWin.png");
	}
	else
	{
		resultLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\BackLose.png");
	}
	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();
}


void Result::Update()
{
	//  画面遷移（フェードイン）が終了しているなら
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN)||Input::GetController(Input::b,Input::PRESSED))
		{
			m_Transition->FadeOut();
		}
	}

	// 画面遷移(フェードアウト)が終了してるか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
	}
}
