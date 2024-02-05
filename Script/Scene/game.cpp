#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"

#include "../GameObject/camera.h"
#include "../GameObject/WallDodgeCamera.h"
#include "../GameObject/field.h"
#include "../GameObject/player.h"
#include "../GameObject/box.h"
#include "../GameObject/cylinder.h"
#include "../GameObject/Polygon2D.h"


#include "../Scene/game.h"
#include "../System/input.h"
#include "../Component/audio.h"

#include "../System/manager.h"
#include "../System/utility.h"
#include "../GameObject/transition.h"

#include "../Scene/result.h"

#include "../GameObject/AttackObject.h"
#include "../GameObject/goal.h"
#include "../GameObject/bullet.h"
#include "../GameObject/enemy.h"
#include "../GameObject/battery.h"
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

//すぐ消す
#include "../Component/Obb.h"

#include <fstream>
using namespace DirectX::SimpleMath;

// ゲーム初期処理
void Game::Init()
{
	Bullet::Load();
	Explosion::Load();
	Slash::Load();
	Puntch::Load();
	HealEffect::Load();

	AddGameObject<Camera>(0);
	AddGameObject<WallDodgeCamera>(1);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	//AddGameObject<Score>(3);
	//AddGameObject<AttackObject>(1);

	// 敵追加
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}
	//AddGameObject<Boss>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(5.0f, 0.0f, -25.0f));
	}
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(10.0f, 0.0f, 15.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(-5.0f, 0.0f, -25.0f));
	}
	{
		GameObject* enemy = AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3(-10.0f, 0.0f, 15.0f));
		enemy->SetRotation({ 0.0f,3.0f,0.0f });
	}

	AddGameObject<Player>(1);
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

	//// チェック完了
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(-11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

	//// チェック完了
	//{
	//	Box* box = AddGameObject<Box>(1);
	//	box->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	//	box->SetScale(Vector3(9.0f, 3.0f, 1.0f));
	//}

	//// チェック完了
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

	// チェック完了
	//{
	//	Goal* goal = AddGameObject<Goal>(1);
	//	goal->SetPosition(Vector3(0.0f, 10.5f, -10.0f));
	//	goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	//}


	GameObject* audioobj = AddGameObject<GameObject>(0);
	audioobj->AddComponent<Audio>()->InitMaster();
	audioobj->GetComponent<Audio>()->Load("asset\\audio\\BGM\\Battle.wav");
	audioobj->GetComponent<Audio>()->Play(true);
	//AddGameObject<Battery>(1)->SetPosition(Vector3(20.0f, 0.0f, 5.0f));

	// 画面遷移オブジェクトを登録
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// ゲーム終了処理
void Game::Uninit()
{
}

// ゲーム更新処理
void Game::Update()
{
	// フェードインが終了しているか？	
//	if (m_Transition->GetState() == Transition::State::Stop) {
//		if (Input::GetKeyTrigger(VK_RETURN))
//		{
//			m_Transition->FadeOut();
//		}
//	}

	// ゴールしていないのであれば
	if (m_Goal==false)
	{
		Enemy* enemy = GetGameObject<Enemy>();
		Player* player = GetGameObject<Player>();
		Boss* boss = GetGameObject<Boss>();

		// ゴールした際にゴールオブジェクトは削除される
		if ((enemy == nullptr && boss == nullptr) || player == nullptr)
		{
			m_Goal = true;
			//Camera* camera = GetGameObject<Camera>();
			//Player* player = GetGameObject<Player>();
			//Vector3 rot = player->GetRotation();
			//rot.y = atan2(camera->GetPosition().x - player->GetPosition().x, camera->GetPosition().z - player->GetPosition().z);
			//player->SetRotation(rot);
			//player->SetAnimName2("Dance");
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

	if (Input::GetController(Input::b, Input::PRESSED))
	{
		HealEffect* slash = AddGameObject<HealEffect>(2);
		slash ->SetPosition(Vector3(-4.0f, 3.0f, 0.0f));
		//slash->SetColor(White);
	}
	
	if (Input::GetController(Input::y, Input::PRESSED))
	{
		Slash* slash = AddGameObject<Slash>(2);
		slash ->SetPosition(Vector3(-4.0f, 3.0f, 0.0f));
		slash->SetColor(Red);
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
