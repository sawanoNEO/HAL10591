#include "../System/main.h"
#include "../System/manager.h"

#include "../System/input.h"

#include "../Scene/title.h"

#include "../Scene/game.h"

#include "../Component/shader.h"
#include "../Component/sprite.h"
#include "../Component/audio.h"

#include "../GameObject/transition.h"

#include "result.h"

using namespace DirectX;
bool Result::m_Winflg = false;

void Result::Init()
{
	GameObject* resultLogo = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	resultLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_BGM = resultLogo->AddComponent<Audio>();

	/////////////////////�N���A�����s���ŕ\������摜�ƋȂ�ς���
	if (m_Winflg == true)
	{
		m_BGM->Load("asset\\audio\\BGM\\Winning_Road.wav");

		resultLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\BackWin.png");
		GameObject* resultText = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
		resultText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		resultText->AddComponent<Sprite>()->Init(250.0f,100.0f, 735*2, 130*2, "asset\\texture\\GAMECLEAR.png");
	}
	else
	{
		m_BGM->Load("asset\\audio\\BGM\\�����Ȍ��e.wav");
		resultLogo->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\BackLose.png");
		GameObject* resultText = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
		resultText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
		resultText->AddComponent<Sprite>()->Init(250.0f, 100.0f, 706*2, 130*2, "asset\\texture\\GAMEOVER.png");
	}

	GameObject* retryText = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	retryText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	retryText->AddComponent<Sprite>()->Init(450.0f, 500.0f, 286, 133, "asset\\texture\\Retry.png");

	GameObject* titleText = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	titleText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleText->AddComponent<Sprite>()->Init(450.0f, 700.0f, 343, 107, "asset\\texture\\Title.png");

	GameObject* exitText = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	exitText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	exitText->AddComponent<Sprite>()->Init(450.0f, 900.0f, 217, 109, "asset\\texture\\Exit.png");		

	m_Arrow = AddGameObject<GameObject>(3);			// 3�̓��C���ԍ�
	m_Arrow->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_Arrow->AddComponent<Sprite>()->Init(350.0f, 500.0f, 494 / 6, 790 / 6, "asset\\texture\\Arrow.png");

	m_Transition = AddGameObject<Transition>(3);					// 3�̓��C���ԍ�
	m_Transition->FadeIn();
	m_BGM->SetVolume(0.0f);
	m_BGM->FadeIn();
	m_BGM->Play(true);
}


void Result::Update()
{
	m_Arrow->GetComponent<Sprite>()->Init(350.0f, 500.0f + (200.0f * m_Cursle), 494 / 6, 790 / 6, "asset\\texture\\Arrow.png");//�J�[�\���ʒu�X�V

	if (m_BGM->GetState() == AUDIOFADEFINISH)
	{
		if (Input::GetController(Input::CROSSDOWN, Input::PRESSED))
		{
			if (m_Cursle == EXIT)
			{
				m_Cursle = 0;
			}
			else
			{
				m_Cursle++;
			}
		}
		else if (Input::GetController(Input::CROSSUP, Input::PRESSED))
		{
			if (m_Cursle == 0)
			{
				m_Cursle = EXIT;
			}
			else
			{
				m_Cursle--;
			}
		}
	}
	//  ��ʑJ�ځi�t�F�[�h�C���j���I�����Ă���Ȃ�
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN)||Input::GetController(Input::b,Input::PRESSED))
		{
			m_Transition->FadeOut();
			m_BGM->FadeOut();
		}
	}

	// ��ʑJ��(�t�F�[�h�A�E�g)���I�����Ă邩�H
	if (m_Transition->GetState() == Transition::State::Finish &&
		m_BGM->GetState() == AudioState::AUDIOFADEFINISH)
	{
		switch (m_Cursle)
		{
		case RETRY:
			Manager::SetScene<Game>();
			break;
		case TITLE:
			Manager::SetScene<Title>();
			break;
		case EXIT:
		Manager::GameExit();
			break;
		default:
			break;
		}
	}
}
