#include "EStateAttack.h"
#include "EStateChase.h"

#include "../Rigidbody.h"
#include "../Colider.h"

#include "../../GameObject/gameobject.h"
#include "../../GameObject/player.h"

#include "../../Scene/scene.h"
#include "../../System/manager.h"

#include "../../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void EStateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	m_AttackObj = scene->AddGameObject<GameObject>(1);
	m_AttackObj->AddComponent<Colider>();
	m_AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	m_AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	m_Startup = 22;
	m_ActiveFrames = 5;
	m_Recovery = 21;
	m_cnt = 0;
	m_Power = 125;
}

void EStateAttack::Enter()
{
	m_GameObject->SetAnimName2("Attack");
	m_Startup = 22;
	m_ActiveFrames = 5;
	m_Recovery = 21;
	m_cnt = 0;
	m_hit = false;
	m_DoubleAttack = false;
}

void EStateAttack::Exit()
{
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



	///////�������e�J�E���g���̏���

	if (m_cnt < m_Startup)
	{
	}
	else if (m_cnt < m_Startup + m_ActiveFrames)/////�U�����肪�o�Ă��鎞�ԁB���������B
	{
		
		//�p�x����
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
		if (m_cnt == m_Startup)
		{
			Vector3 vec = forward * 100;
			rb->AddForce(vec, ForceMode::Impulse);
		}

		//�U�����������Ă��邩���肷��
		Colider* hitobj = m_AttackObj->GetComponent<Colider>()->CollisionAABB(m_AttackObj->GetComponent<Colider>()->GetAABB(), col);

		if (hitobj!=nullptr&&hitobj->GetTug() == PLAYER&&
			m_hit==false)//���̍U���ŕ�����q�b�g����̂�h��
		{
			player->Damage(m_Power);
			m_hit = true;
		}
	}
	else if (m_cnt == m_Startup + m_ActiveFrames)
	{
		if (m_DubleAttackRate >= rand() % 100&&m_DoubleAttack==false)
		{
			m_GameObject->SetAnimName2("Attack3");
			m_Startup = 16;
			m_ActiveFrames = 5;
			m_Recovery = 21;
			m_cnt = 0;
			m_hit = false;
			m_DoubleAttack=true;
		}
	}
	//else if (cnt < Startup + ActiveFrames + Recovery)         //�U����̍d�����ԁB���̊Ԃ̈��̃^�C�~���O�ł��������ǍU���{�^���������ƘA���ɂȂ�
	//{
	//	if (Input::GetController(Input::R1, Input::PRESSED, 15) != -1 &&
	//		player->GetST() > 0.0f &&
	//		cnt > Startup + ActiveFrames + (Recovery / 3))     //�d�����n�܂��Ă����͘A���Ɉڍs�ł��Ȃ�
	//	{
	//		cnt = Startup - 10;
	//		player->SetFrame1(0);
	//		//player->STUse(17.0f);
	//	}
	//}
	
	m_cnt++;
}

void EStateAttack::StateChange()
{
	if (m_cnt > m_Startup + m_ActiveFrames + m_Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<EStateChase>());
	}

}

void EStateAttack::Draw()
{
	ImGui::Begin("EnemyAttaState");
	ImGui::SliderInt("Frame", &m_Recovery, 0, 50);
	ImGui::End();
}