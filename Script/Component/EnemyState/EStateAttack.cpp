#include "EStateAttack.h"
#include "EStateChase.h"

#include "../Rigidbody.h"
#include "../Colider.h"

#include "../../GameObject/gameobject.h"
#include "../../GameObject/player.h"
#include "../../GameObject/Effect/Slash.h"

#include "../../Scene/scene.h"
#include "../../System/manager.h"

#include "../../ImGui/imguimanager.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace DirectX::SimpleMath;
using namespace std;

void EStateAttack::Init()
{

}

void EStateAttack::Init(int _startup, int _activeframes, int _recovery, int _power, int _doubleattackrate, DirectX::SimpleMath::Vector3 _scale)
{
	m_Startup.push_back(_startup);
	m_ActiveFrames.push_back(_activeframes);
	m_Recovery.push_back(_recovery);
	m_Power.push_back(_power);
	m_DubleAttackRate.push_back(_doubleattackrate);
	m_AttackObj->SetScale(_scale);
}

void EStateAttack::Enter()
{
	m_GameObject->SetAnimName2("Attack");
	m_cnt = 0;
	m_hit = false;
	m_DoubleAttack = false;
}

void EStateAttack::Exit()
{
	m_AttackNum = 0;
	m_GameObject->SetAnimSpeed(1.0);
}

void EStateAttack::Init(const char* FilePath)
{
	Scene* scene = Manager::GetScene();
    m_AttackObj = scene->AddGameObject<GameObject>(1);
    m_AttackObj->AddComponent<Colider>();
    m_AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
    m_AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });

	////csvファイルを読み込んで各パラメータを代入
	ifstream file(FilePath);
	assert(file.is_open()&&"Fileが見つかりませんでした");
	string line;

	getline(file, line);
	stringstream term(line);
	string temp;
	unordered_map<string,int> id;
	int i = 0;
	while (getline(term, temp, ','))
	{
		id[temp] = i;
		i++;
	}
	while (getline(file, line))
	{
        string cell;
		stringstream ss(line);
		vector<int> row;
		while (getline(ss, cell, ',')) 
		{
			if (cell.size() != 0)
			{
			   row.push_back(stoi(cell));
			}
			else
			{
				row.push_back(0);
			}
		}
		m_Startup.push_back(row[id["StartUp"]]);
		m_ActiveFrames.push_back(row[id["ActiveFrames"]]);
		m_Recovery.push_back(row[id["Recovery"]]);
		m_Power.push_back(row[id["Power"]]);
		m_DubleAttackRate.push_back(row[id["DoubleAttackRate"]]);
	}
	file.close();
}

void EStateAttack::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	if (!player)
	{
		return;
	}

	Vector3 forward = m_GameObject->GetForward();
	Vector3 playerpos = player->GetPosition();
	bool hitCheck = false;
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();
	Colider* col = player->GetComponent<Colider>();
	Vector3 rotation = m_GameObject->GetRotation();
	Vector3 vec = playerpos - m_GameObject->GetPosition();
	vec.Normalize();

	m_AttackObj->SetPosition(m_GameObject->GetPosition() + forward * 3);



	///////↓↓↓各カウント毎の処理

	if (m_cnt < m_Startup[m_AttackNum])
	{
	}
	else if (m_cnt == m_Startup[m_AttackNum])
	{
		m_Slash = scene->AddGameObject<Slash>(1);
		m_Slash->SetColor(White);
		Vector3 pos = m_GameObject->GetPosition() + m_GameObject->GetForward() * 4;
		pos.y += 2.0;
		//slash->SetPosition(player->GetPosition() + player->GetForward() * 5);
		m_Slash->SetPosition(pos);
		Vector3 rot = { 0.0,0.0,0.0 };
		rot.z += cos(90 * 3.14 / 180);
		m_Slash->SetRotation(rot);

		//拳が空を切る音を再生
		m_SENumber = rand() % 3;//再生する音を決定
		switch (m_SENumber)
		{
		case 0:
			m_GameObject->PlaySE("Swing1",false);
			break;
		case 1:
			m_GameObject->PlaySE("Swing2", false);
			break;
		case 2:
			m_GameObject->PlaySE("Swing3", false);
			break;
		default:
			break;
		}
	}
	else if (m_cnt < m_Startup[m_AttackNum] + m_ActiveFrames[m_AttackNum])/////攻撃判定が出ている時間。持続部分。
	{
		
		//角度調整
		float gap = forward.Dot(vec);
		if (gap > 0 && gap < 0.95)
		{
			rotation.y += 0.3;
		}
		else if (gap<0 && gap>-0.95)
		{
			rotation.y -= 0.3;
		}
		m_GameObject->SetRotation(rotation);
		if (m_cnt == m_Startup[m_AttackNum])
		{
			Vector3 vec = forward * 100;
			rb->AddForce(vec, ForceMode::Impulse);
		}

		//攻撃が当たっているか判定する
		Colider* hitobj = m_AttackObj->GetComponent<Colider>()->CollisionAABB(m_AttackObj->GetComponent<Colider>()->GetAABB(), col);

		//当たっていた場合の処理
		if (hitobj!=nullptr&&hitobj->GetTug() == PLAYER&&
			m_hit==false)//一回の攻撃で複数回ヒットするのを防ぐ
		{
			if (player->Damage(m_Power[m_AttackNum]) == true)
			{
				switch (m_SENumber)
				{
				case 0:
					m_GameObject->StopSE("Swing1");
					m_GameObject->PlaySE("Puntch1", false);
					break;
				case 1:
					m_GameObject->StopSE("Swing2");
					m_GameObject->PlaySE("Puntch2", false);
					break;
				case 2:
					m_GameObject->StopSE("Swing3");
					m_GameObject->PlaySE("Puntch3", false);
					break;
				default:
					break;
				}
				m_Slash->SetColor(Red);
			}
			m_hit = true;
		}
	}
	else if (m_cnt == m_Startup[m_AttackNum] + m_ActiveFrames[m_AttackNum])
	{
		if (m_DubleAttackRate[m_AttackNum] >= rand() % 100 && m_DoubleAttack == false)
		{
			m_GameObject->SetAnimName2("Attack3");
			m_GameObject->SetAnimSpeed(0.5);
			m_AttackNum++;
			//m_Startup = 32;
			//m_ActiveFrames = 10;
			//m_Recovery = 42;
			m_cnt = 0;
			m_hit = false;
			m_DoubleAttack=true;
		}
	}

	
	m_cnt++;
}

void EStateAttack::StateChange()
{
	if (m_cnt > m_Startup[m_AttackNum] + m_ActiveFrames[m_AttackNum] + m_Recovery[m_AttackNum])
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
	}
}

void EStateAttack::Draw()
{
#if _DEBUG
	ImGui::Begin("EnemyAttaState");
	ImGui::SliderInt("Frame", &m_Recovery[m_AttackNum], 0, 50);
	ImGui::End();
#endif
}