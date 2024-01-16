#include "EStateNone.h"
#include "EStateChase.h"

#include "../../GameObject/gameObject.h"
#include "../../GameObject/player.h"

#include "../../System/manager.h"

#include "../../Scene/scene.h"

using namespace DirectX::SimpleMath;

void EStateNone::Enter()
{
	m_GameObject->SetAnimName2("Idle");

}

void EStateNone::Exit()
{
}

void EStateNone::StateUpdate()
{

}

void EStateNone::StateChange()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (player)
	{
		Vector3 playerPos = player->GetPosition();

		if (SearchPlayer(playerPos, m_GameObject->GetPosition(), 180.0f, 15.0f))//プレイヤーが視界内に居れば追跡を開始
		{
			m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
		}
	}
}

void EStateNone::Draw()
{
}

bool EStateNone::SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length)
{
	//プレイヤーと自身を結ぶベクトル
	Vector3 vecobj;
	vecobj = playerpos - m_pos;

	//プレイヤーとの距離を求める
	float objlength = vecobj.Length();

	//許容距離の範囲外かチェック
	if (objlength > length)
	{
		return false;
	}

	//視線ベクトル
	Vector3 vecview;
	//vecview = playerpos - m_pos;
	vecview = m_GameObject->GetForward() * length;

	//正規化
	vecview.Normalize();
	vecobj.Normalize();

	//内積を計算
	float dotobj = vecview.Dot(vecobj);//視線ベクトルとプレイヤーへのベクトルの内積

	////回転//これでは動かない判断をしたが恐らく合っているので記念に
	//Matrix mtx;
	//mtx = mtx.CreateRotationZ(fov / 2.0f);

	//Vector3 vecrotview=m_GameObject->GetForward();//視野範囲
	//vecrotview.y=

	//vecrotview = vecview.Transform(vecview, mtx);

	//float dotrotview = vecview.Dot(vecrotview);
	float dotrotview = cos(fov / 2 * 3.14 / 180.0f);

	if (dotrotview <= dotobj)
	{
		return true;
	}

	return false;
}