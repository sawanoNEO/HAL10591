#include "EStateWaitandSee.h"
#include "EStateNone.h"

#include "../Rigidbody.h"

#include "../../GameObject/player.h"
#include "../../GameObject/ColiderLooker.h"

#include "../../Scene/scene.h"

#include "../../ImGui/imguimanager.h"

#include "../../System/manager.h"

#define radian 3.14/180

using namespace DirectX::SimpleMath;

void EStateWaitandSee::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (player == nullptr)
	{
		return;
	}
	//Vector3 vec = m_GameObject->GetPosition() - player->GetPosition();//自身とプレイヤーとのベクトル
	//vec.Normalize();


	m_HowLong = m_RandLowerLimit + rand() % (m_RandUpperLimit - m_RandLowerLimit);
	m_GameObject->SetAnimSpeed(-1.0f);
	m_GameObject->SetAnimName2("Walk");
}

void EStateWaitandSee::Exit()
{
	m_GameObject->SetAnimSpeed(1.0f);
	m_Count = 0;
	m_SideMoveflg = false;
}

void EStateWaitandSee::StateUpdate()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (player == nullptr)
	{
		return;
	}
	Vector3 rotation = m_GameObject->GetRotation();
	Rigidbody* rb = m_GameObject->GetComponent<Rigidbody>();
	Vector3 position = m_GameObject->GetPosition();
	Vector3 playerpos = player->GetPosition();
	Vector3 vec = DirectX::XMVector3Normalize(playerpos - position);
	
	//常にプレイヤーを見続ける
	rotation.y = atan2(vec.x, vec.z);
	m_GameObject->SetRotation(rotation);
	
	if (m_Count < m_HowLong && m_SideMoveflg == false)
	{
		//自分から目標位置までのベクトルを計算
		/*m_WalkVec = m_BackPoint - position;
		m_WalkVec.Normalize();*/
		m_WalkVec = -m_GameObject->GetForward();
	}
	else if (m_Count >= m_HowLong && m_SideMoveflg == false)
	{
		m_SideMoveflg = true;
		m_Count = 0;
		//乱数によって左右どちらに動くかを決定
		m_Direction = rand() % 2 == 1 ? 1 : -1;
		if (m_Direction==1)
		{
			m_GameObject->SetAnimName2("EnemyRightStrafe");
		}
		else
		{
			m_GameObject->SetAnimName2("EnemyLeftStrafe");
		}
		m_GameObject->SetAnimSpeed(1.0f);
		m_HowLong = m_RandLowerLimit + rand() % (m_RandUpperLimit - m_RandLowerLimit);
	}
	else if (m_Count < m_HowLong&&m_SideMoveflg==true)
	{
		//自身の横向きベクトルを基に進む方向を決定
		m_WalkVec = m_GameObject->GetSide();
		m_WalkVec *= m_Direction;
	}
	
	rb->AddForce(m_WalkVec * 350, ForceMode::Force);
	m_Count++;
}

void EStateWaitandSee::StateChange()
{
	//暫定的な処理
	if (m_Count>m_HowLong)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateNone>());
	}
}

void EStateWaitandSee::Draw()
{
#if _DEBUG
	ImGui::Begin("WaitAndSee");
	ImGui::SliderFloat("Distance", &m_Distance, 1.0f, 10.0f);
	ImGui::SliderInt("UPPER", &m_RandUpperLimit, m_RandLowerLimit, 300);
	ImGui::SliderInt("LOWER", &m_RandLowerLimit, 10, m_RandUpperLimit);
	ImGui::Text("Walkvectol%f,%f,%f", m_WalkVec.x, m_WalkVec.y, m_WalkVec.z);
	ImGui::Text("BackPoint%f,%f,%f", m_BackPoint.x, m_BackPoint.y, m_BackPoint.z);
	ImGui::Text("Direction%i", m_Direction);
	ImGui::Text("zentaiframe%i", m_HowLong);
	ImGui::End();
#endif
}
