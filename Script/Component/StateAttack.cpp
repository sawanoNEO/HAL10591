#include "StateAttack.h"
#include "StateNone.h"
#include "Colider.h"
#include "Rigidbody.h"
#include "audio.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/player.h"
#include "../GameObject/enemy.h"
#include "../GameObject/Boss.h"
#include "../GameObject/camera.h"
#include "../GameObject/Effect/Slash.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void StateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	AttackObj = scene->AddGameObject<GameObject>(1);
	AttackObj->AddComponent<Colider>();
	AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	m_Maxcombo = 2;
	m_combo = 0;
	m_Startup = 22;
	m_ActiveFrames = 5;
	m_Recovery = 23;
	m_Count = 0;
	m_Power = 45;
	m_Staminaconsumption = 17.0;
}

void StateAttack::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimName2("Attack");
	m_Count = 0;
	player->SetAnimSpeed(2.0f);
	//player->SetFrame1();
}

void StateAttack::Exit()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimSpeed(1.0f);
}

void StateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = player->GetForward();
	std::vector<Enemy*> enemys = scene->GetGameObjects<Enemy>();
	std::vector<Colider*> col;//オブジェクトの当たり判定を取得する
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	bool hit = false;
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	std::unordered_map<const char*,Audio*> se = player->GetSEs();

	AttackObj->SetPosition(player->GetPosition()+forward*3);

	////敵のコライダーを取得
	enemys = Enemy::GetEnemys();
	col.resize(enemys.size());
	for (int i=0;i<enemys.size();i++)
	{
		col[i] = enemys[i]->GetComponent<Colider>();
	}
	//col.resize(enemys.size()+Bosses.size());
	//for (int i = 0; i < enemys.size(); i++)
	//{
	//	col[i] = enemys[i]->GetComponent<Colider>();
	//}
	//for (int i = 0; i < Bosses.size(); i++)
	//{
	//	col[i + enemys.size()] = Bosses[i]->GetComponent<Colider>();
	//}

	if (m_Count < m_Startup)//攻撃の予備動作
	{

		m_Staminaconsumption = 17.0f;

		//既にヒットしたという判定をリセット
		for (const auto enemy : enemys)
		{
			enemy->HitReset();
		}

	}
	else if (m_Count < m_Startup +m_ActiveFrames)/////攻撃判定が出ている時間。持続部分。
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));

		//攻撃が発生する瞬間だけ行う処理
		if (m_Count == m_Startup)
		{
			//エフェクトとなるオブジェクトを前方に生成
			slash = scene->AddGameObject<Slash>(1);
			slash->SetColor(White);
			Vector3 pos = player->GetPosition() + player->GetForward() * 6;
			pos.y += 2.0;
			//slash->SetPosition(player->GetPosition() + player->GetForward() * 5);
			slash->SetPosition(pos);
			Vector3 rot = { 0.0,0.0,0.0 };
			rot.z += cos(90 * 3.14 / 180);
			slash->SetRotation(rot);

			player->STUse(m_Staminaconsumption);
			Vector3 vec = player->GetForward() * 100.0f;
			if (Input::GetStickState())
			{
				Vector3 vec = player->GetpromissDirection() * 100.0f;
			}
			rb->AddForce(vec, ForceMode::Impulse);

			//拳が空を切る音を再生
			m_SENumber = rand() % 3;//再生する音を決定
			switch (m_SENumber)
			{
			case 0:
				se["Swing1"]->Play(false);
				break;
			case 1:
				se["Swing2"]->Play(false);
				break;
			case 2:
				se["Swing3"]->Play(false);
				break;
			default:
				break;
			}
		}

		if (enemys.size() == 0)
		{
			return;
		}

		//敵と攻撃判定が接触しているか判定する
		for (int i = 0; i < enemys.size(); i++)
		{
			Colider* hitobj = col[i]->CollisionAABB(AttackObj->GetComponent<Colider>()->GetAABB(), col[i]);

			if (hitobj != nullptr && hitobj->GetTug() == ENEMY)
			{
			    //攻撃がヒットした時の処理
				if (enemys[i] && !hit)
				{
					switch (m_SENumber)
					{
					case 0:
						se["Swing1"]->Stop();
						se["Puntch1"]->Play(false);
						break;
					case 1:
						se["Swing2"]->Stop();
						se["Puntch2"]->Play(false);
						break;
					case 2:
						se["Swing3"]->Stop();
						se["Puntch3"]->Play(false);
						break;
					default:
						break;
					}
					Vector3 enemypos = enemys[i]->GetPosition();
					enemypos.y += 1.0f;
					enemys[i]->Damage(m_Power);
					slash->SetColor(Red);
					//slash->SetPosition(enemypos);
				}
			}
		}
	}
	else if (m_Count < m_Startup +m_ActiveFrames + m_Recovery)         //攻撃後の硬直時間。この間の一定のタイミングでもういちど攻撃ボタンを押すと連撃になる
	{
		if (Input::GetController(Input::R1, Input::PRESSED,15)!=-1 &&
			player->GetST() > 0.0f &&
			m_Count > m_Startup +m_ActiveFrames + (m_Recovery / 3))     //硬直が始まってすぐは連撃に移行できない
		{
			m_Count = m_Startup - 10;
			player->SetFrame1(0);
			//player->STUse(17.0f);
		}
	}
	
	m_Count++;
}

void StateAttack::StateChange()
{
	if (m_Count > m_Startup +m_ActiveFrames + m_Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateAttack::Draw()
{
#if _DEBUG
	ImGui::Begin("StateAttack");
	ImGui::SliderInt("Recovery", &m_Recovery, -20, 20);
	ImGui::End();
#endif
}
