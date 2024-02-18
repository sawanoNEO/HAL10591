#include "EStateDamage.h"
#include "EStateNone.h"
#include "EStateChase.h"

#include "../../System/manager.h"
#include "../../Scene/scene.h"

#include "../../GameObject/gameObject.h"
#include "../../GameObject/player.h"

#include "../../ImGui/imguimanager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace DirectX::SimpleMath;
using namespace std;

void EStateDamage::Init(const char* FilePath)
{
	////csvファイルを読み込んで各パラメータを代入
	ifstream file(FilePath);
	assert(file.is_open() && "Fileが見つかりませんでした");
	string line;

	getline(file, line);
	stringstream term(line);
	string temp;
	unordered_map<string, int> id;
	int i = 0;
	while (getline(term, temp, ','))
	{
		id[temp] = i;
		i++;
	}
	
	getline(file, line);
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
	m_Recover = row[id["DamageRecover"]];
	file.close();
}

void EStateDamage::Enter()
{
	if (m_Hitting == false)
	{
		m_GameObject->SetAnimName2("Impact");
		m_Count = 0;
	}
	m_Hitting = true;
}

void EStateDamage::Exit()
{
	if (m_Count > m_Recover)
	{
		m_Hitting = false;
	}
}

void EStateDamage::StateUpdate()
{
	m_Count++;
}

void EStateDamage::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 pos = m_GameObject->GetPosition();
	Vector3 playerpos = player->GetPosition();

	//ダメージリアクションが終わったら状態遷移
	if (m_Count > m_Recover && (pos - playerpos).Length() < 5.0)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
	}
	else if (m_Count > m_Recover)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
}

void EStateDamage::Draw()
{
#if _DEBUG
	ImGui::Begin("EstateDamege");
	ImGui::SliderInt("Recover", &m_Recover, 20, 42);
	ImGui::End();
#endif
}
