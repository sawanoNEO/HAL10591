#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "field.h"
#include "player.h"
#include "box.h"
#include "cylinder.h"

#include "game.h"
#include "input.h"
#include "audio.h"
#include "Polygon2D.h"

#include "manager.h"
#include "transition.h"

#include "result.h"

#include "goal.h"

#include "utility.h"

#include "bullet.h"

#include "enemy.h"

#include "battery.h"

#include "sky.h"

#include "score.h"
#include "ST.h"
#include "HP.h"

#include "explosion.h"

using namespace DirectX::SimpleMath;

// ゲーム初期処理
void Game::Init()
{
	Bullet::Load();
	Explosion::Load();

	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	AddGameObject<Player>(1);
	//AddGameObject<Score>(3);
	AddGameObject<ST>(3);
	AddGameObject<HP>(3);

	// 敵追加
	AddGameObject<Enemy>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(5.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(10.0f, 0.0f, 5.0f));
	AddGameObject<Enemy>(1)->SetPosition(Vector3(15.0f, 0.0f, 5.0f));

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-5.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(3.0f, 1.0f, 3.0f));
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-11.0f, 0.0f, 5.0f));
		box->SetScale(Vector3(3.0f, 2.0f, 3.0f));
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(-11.0f, 0.0f, 11.0f));
		box->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	}
	
	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(15.0f, 0.0f, 0.0f));
		box->SetScale(Vector3(5.0f, 1.0f, 5.0f));
	}

	// チェック完了
	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPosition(Vector3(-11.0f, 0.0f, 20.0f));
		cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	}

	// チェック完了
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		box->SetScale(Vector3(9.0f, 3.0f, 1.0f));
	}

	// チェック完了
	{
		Cylinder* cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
		cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	}

	// チェック完了
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(0.0f, 10.5f, -10.0f));
		goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	}

	AddGameObject<Battery>(1)->SetPosition(Vector3(20.0f, 0.0f, 5.0f));

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
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();

		// ゴールした際にゴールオブジェクトは削除される
		if (goal == nullptr)
		{
			m_Goal = true;

			// ２秒後にスレッドを生成してフェードアウト開始
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	// フェードアウトが終了しているか？
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}
