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

		if (SearchPlayer(playerPos, m_GameObject->GetPosition(), 180.0f, 15.0f))//�v���C���[�����E���ɋ���ΒǐՂ��J�n
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
	//�v���C���[�Ǝ��g�����ԃx�N�g��
	Vector3 vecobj;
	vecobj = playerpos - m_pos;

	//�v���C���[�Ƃ̋��������߂�
	float objlength = vecobj.Length();

	//���e�����͈̔͊O���`�F�b�N
	if (objlength > length)
	{
		return false;
	}

	//�����x�N�g��
	Vector3 vecview;
	//vecview = playerpos - m_pos;
	vecview = m_GameObject->GetForward() * length;

	//���K��
	vecview.Normalize();
	vecobj.Normalize();

	//���ς��v�Z
	float dotobj = vecview.Dot(vecobj);//�����x�N�g���ƃv���C���[�ւ̃x�N�g���̓���

	////��]//����ł͓����Ȃ����f�����������炭�����Ă���̂ŋL�O��
	//Matrix mtx;
	//mtx = mtx.CreateRotationZ(fov / 2.0f);

	//Vector3 vecrotview=m_GameObject->GetForward();//����͈�
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