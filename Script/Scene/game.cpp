#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"

#include "../GameObject/camera.h"
#include "../GameObject/field.h"
#include "../GameObject/player.h"
#include "../GameObject/box.h"
#include "../GameObject/Polygon2D.h"


#include "../Scene/game.h"
#include "../Scene/result.h"
#include "../System/input.h"
#include "../Component/audio.h"
#include "../Component/shader.h"
#include "../Component/sprite.h"
#include "../Component/StateMachine.h"
#include "../System/AiSceneSmartPtr.h"

#include "../System/manager.h"
#include "../System/utility.h"
#include "../GameObject/transition.h"

#include "../Scene/result.h"

#include "../GameObject/goal.h"
#include "../GameObject/enemy.h"
#include "../GameObject/sky.h"
#include "../GameObject/score.h"
#include "../GameObject/ST.h"
#include "../GameObject/HP.h"
#include "../GameObject/ColiderLooker.h"
#include "../GameObject/explosion.h"
#include "../GameObject/Boss.h"

#include"../GameObject/Effect/Slash.h"
#include"../GameObject/Effect/Puntch.h"
#include"../GameObject/Effect/HealEffect.h"

#include "../ImGui/imguimanager.h"

#include <fstream>
using namespace DirectX::SimpleMath;

void Game::ChangeLoadImage()
{
	m_LoadCount++;
	Renderer::Begin();
	switch (m_LoadCount%4)
	{
	case 0:
		m_LoadImage->GetComponent<Sprite>()->Init(1200, 900, 566, 115, "asset\\texture\\Now Loading.png");
		break;
	case 1:
		m_LoadImage->GetComponent<Sprite>()->Init(1200, 900, 587, 115, "asset\\texture\\Now Loading1.png");
		break;
	case 2:
		m_LoadImage->GetComponent<Sprite>()->Init(1200, 900, 612, 115, "asset\\texture\\Now Loading2.png");
		break;
	case 3:
		m_LoadImage->GetComponent<Sprite>()->Init(1200, 900, 637, 115, "asset\\texture\\Now Loading3.png");
		break;
		
	default:
		break;
	}
	m_LoadImage->Update();
	m_LoadBackImage->DrawBase(Matrix::Identity);
	m_LoadImage->DrawBase(Matrix::Identity);
	Renderer::End();
}

void Game::LoadImageDraw()
{
	Renderer::Begin();

	m_LoadBackImage = AddGameObject<GameObject>(3);			// 3はレイヤ番号
	m_LoadBackImage->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_LoadBackImage->AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\TitleBack.jpg");
	m_LoadBackImage->UpdateBase();
	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = false;
	m_LoadBackImage->GetComponent<Sprite>()->SetMaterial(material);
	m_LoadBackImage->DrawBase(Matrix::Identity);

	m_LoadImage = AddGameObject<GameObject>(3);
	m_LoadImage->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	m_LoadImage->AddComponent<Sprite>()->Init(1200.0f, 900.0f, 566, 115, "asset\\texture\\Now Loading.png");
	m_LoadImage->UpdateBase();

	m_LoadImage->DrawBase(Matrix::Identity);
	Renderer::End();
}

// ゲーム初期処理
void Game::Init()
{
	//LoadImageDraw();
	Explosion::Load();
	Slash::Load();
	Puntch::Load();
	HealEffect::Load();
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Score>(3);

	GameObject* portion = AddGameObject<GameObject>(3);
	portion->AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");
	portion->AddComponent<Sprite>()->Init(100.0f, 750.0f, 500 / 2, 500 / 2, "asset\\texture\\ポーション.png");
	// 敵追加	
	//ロードのために一度生成してすぐに削除する
	GameObject* boss = AddGameObject<Boss>(1);
	boss->SetDestroy();
	ChangeLoadImage();
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(0.0f, 0.0f, 30.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}
	ChangeLoadImage();
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(15.0f, 0.0f, -45.0f));
	}
	ChangeLoadImage();
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(10.0f, 0.0f, 35.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}
	ChangeLoadImage();
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(-15.0f, 0.0f, -45.0f));
	}
	ChangeLoadImage();
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(-10.0f, 0.0f, 35.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}
	ChangeLoadImage();
	AddGameObject<Player>(1);
	ChangeLoadImage();
	AddGameObject<ST>(3);
	AddGameObject<HP>(3);

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-54.0f, 0.0f, 0.0f));
		box->SetScale(Vector3(5.0f, 5.0f, 50.0f));
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(0.0f, 0.0f, 54.0f));
		box->SetScale(Vector3(50.0f, 5.0f, 5.0f));
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(0.0f, 0.0f, -54.0f));
		box->SetScale(Vector3(50.0f, 5.0f, 5.0f));
	}
	
	//チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(15.0f, 0.0f, 0.0f));
		box->SetScale(Vector3(5.0f, 1.0f, 5.0f));
	}

	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(54.0f, 0.0f, 0.0f));
		box->SetScale(Vector3(5.0f, 5.0f, 50.0f));
	}
	ChangeLoadImage();
	
	GameObject* audioobj = AddGameObject<GameObject>(0);
	audioobj->AddComponent<Audio>()->InitMaster();
	audioobj->GetComponent<Audio>()->Load("asset\\audio\\BGM\\Battle.wav");
	audioobj->GetComponent<Audio>()->FadeIn();
	audioobj->GetComponent<Audio>()->SetVolume(0.0f);
	audioobj->GetComponent<Audio>()->Play(true);

	// 画面遷移オブジェクトを登録

	m_Transition = AddGameObject<Transition>(3);
	m_LoadBackImage->SetDestroy();
	m_LoadImage->SetDestroy();
	m_Transition->FadeIn();
}

// ゲーム終了処理
void Game::Uninit()
{
	AiSceneSmartPtr::UnInitAll();
}

// ゲーム更新処理
void Game::Update()
{
	// ゴールしていないのであれば
	if (m_Goal==false)
	{
		Enemy* enemy = GetGameObject<Enemy>();
		Player* player = GetGameObject<Player>();

		if (enemy == nullptr&&m_BossApearance == false)///雑魚が全滅したらボス登場
		{
			AddGameObject<Boss>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
			Score* score = GetGameObject<Score>();
			GetGameObject<Boss>()->SetRotation(Vector3(0.0f, 3.0f, 0.0f));
			m_BossApearance = true;
		}
		
		Boss* boss = GetGameObject<Boss>();
		//最後に残ったのがプレイヤーか敵かでエンドを分岐
		if (enemy == nullptr && boss == nullptr)
		{
			m_Goal = true;
			Camera* camera = GetGameObject<Camera>();
			Player* player = GetGameObject<Player>();
			Vector3 rot = player->GetRotation();
			rot.y = atan2(camera->GetPosition().x - player->GetPosition().x, camera->GetPosition().z - player->GetPosition().z);
			player->SetRotation(rot);
			player->SetAnimName2("Dance");
			player->SetAnimSpeed(2.0f);
			Result::SetResult(true);
			player->GetComponent<StateMachine>()->SetEnable(false);
			// ２秒後にスレッドを生成してフェードアウト開始
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
		else if (player == nullptr)
		{
			Result::SetResult(false);
			m_Goal = true;
			// ２秒後にスレッドを生成してフェードアウト開始
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	
	}
	else if (m_Goal == true)
	{
		Camera* camera = GetGameObject<Camera>();
		Player* player = GetGameObject<Player>();
		if (player != nullptr)
		{
			Vector3 rot = player->GetRotation();
			rot.y = atan2(camera->GetPosition().x - player->GetPosition().x, camera->GetPosition().z - player->GetPosition().z);
			player->SetRotation(rot);

		}
	}

	// フェードアウトが終了しているか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}

void Game::Draw()
{
#if _DEBUG
	ImGui::Begin("GameScene");
	if (ImGui::Button("SpawnEnemy"))
	{
		Enemy* enemy=AddGameObject<Enemy>(1);
		Score* score = AddGameObject<Score>(3);
		enemy->SetPosition(Vector3{ 0.0f,1.0f,5.0f });
	}
	if (ImGui::Button("SpawnBoss"))
	{
		Boss* enemy=AddGameObject<Boss>(1);
		enemy->SetPosition(Vector3{ 0.0f,0.0f,5.0f });
		enemy->SetRotation(Vector3{ 0.0f,3.0f,0.0f });
		enemy->SetAnimName2("BossAppearance");
	}
	ImGui::End();
#endif
}
