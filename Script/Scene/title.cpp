#include "../System/main.h"
#include "../System/manager.h"

#include "../System/input.h"

#include "title.h"
#include "game.h"
#include "endless.h"

#include "../Component/shader.h"
#include "../Component/../Component/sprite.h"
#include "../Component/audio.h"

#include "../GameObject/transition.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

int x = 0;

void Title::Init()
{
	m_gameobject = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	m_gameobject->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_gameobject->AddComponent<Sprite>()->Init(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\BlackBack.png");
	m_gameobject->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	GameObject* titleLogo = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	titleLogo->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(200.0f, -100.0f, 998*1.5,494*1.5, "asset\\texture\\タイトルなし.png");

	GameObject* stratText = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	stratText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	stratText->AddComponent<Sprite>()->Init(450.0f, 500.0f, 256,111, "asset\\texture\\Start.png");
	
	//GameObject* endlessText = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	//endlessText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	//endlessText->AddComponent<Sprite>()->Init(450.0f, 700.0f, 395, 111, "asset\\texture\\Endless.png");
	
	GameObject* exitText = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	exitText->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	exitText->AddComponent<Sprite>()->Init(450.0f, 900.0f, 217, 109, "asset\\texture\\Exit.png");

	m_Arrow = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	m_Arrow->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_Arrow->AddComponent<Sprite>()->Init(350.0f, 500.0f, 494/6, 790/6, "asset\\texture\\Arrow.png");



	m_Transition = AddGameObject<Transition>(3);					// 3はレイヤ番号
	m_Transition->FadeIn();

	//BGM再生
	GameObject* audioobj = AddGameObject<GameObject>(0);
	m_BGM = audioobj->AddComponent<Audio>();
	m_BGM->InitMaster();
	m_BGM->Load("asset\\audio\\BGM\\TitleBGM1.wav");
	m_BGM->FadeIn();
	m_BGM->Play(true);
}


void Title::Update()
{
	m_Arrow->GetComponent<Sprite>()->Init(350.0f, 500.0f+(400.0f *m_Cursle), 494 / 6, 790 / 6, "asset\\texture\\Arrow.png");//カーソル位置更新

	//カーソル移動
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
	
	//フェードインが終了後、決定ボタンが押された
	if (m_Transition->GetState() == Transition::State::Stop)
	{
		if (Input::GetController(Input::b,Input::PRESSED)||Input::GetKeyTrigger(VK_RETURN))
		{
			m_Transition->FadeOut();
			m_BGM->FadeOut();
		}
	}


	m_gameobject->GetComponent<Sprite>()->SetVertex(x, x, SCREEN_WIDTH, SCREEN_HEIGHT);
	// 画面遷移が終了してるか？
	//フェードアウト終了後、遷移する画面を決定する
	if (m_Transition->GetState() == Transition::State::Finish&&
		m_BGM->GetState()==AudioState::AUDIOFADEFINISH)
	{
		switch (m_Cursle)
		{
		case START:
			Manager::SetScene<Game>();
			break;
		//case ENDLESS:
		//	Manager::SetScene<Endless>();
		//	break;
		case EXIT:
			Manager::GameExit();
			break;
		default:
			break;
		}
	}
}

void Title::Draw()
{
#if _DEBUG
	ImGui::Begin("title");
	ImGui::Text("%f%f%f", m_gameobject->GetPosition().x, m_gameobject->GetPosition().y, m_gameobject->GetPosition().z);
	ImGui::Text("%i", x);
	ImGui::End();
#endif
}
