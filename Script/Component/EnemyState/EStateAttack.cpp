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

using namespace DirectX::SimpleMath;

void EStateAttack::Init()
{
	Scene* scene = Manager::GetScene();
	m_AttackObj = scene->AddGameObject<GameObject>(1);
	m_AttackObj->AddComponent<Colider>();
	m_AttackObj->GetComponent<Colider>()->Init(DEFAULT, Vector3{ 1.0f,1.0f,1.0f });
	m_AttackObj->SetScale(Vector3{ 4.0f,2.0f,4.0f });
	m_Startup = 17;
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
	m_GameObject->SetAnimSpeed(1.0);
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
	else if (m_cnt == m_Startup)
	{
		slash = scene->AddGameObject<Slash>(1);
		slash->SetColor(White);
		Vector3 pos = m_GameObject->GetPosition() + m_GameObject->GetForward() * 4;
		pos.y += 2.0;
		//slash->SetPosition(player->GetPosition() + player->GetForward() * 5);
		slash->SetPosition(pos);
		Vector3 rot = { 0.0,0.0,0.0 };
		rot.z += cos(90 * 3.14 / 180);
		slash->SetRotation(rot);

		//�������؂鉹���Đ�
		m_SENumber = rand() % 3;//�Đ����鉹������
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

		//�������Ă����ꍇ�̏���
		if (hitobj!=nullptr&&hitobj->GetTug() == PLAYER&&
			m_hit==false)//���̍U���ŕ�����q�b�g����̂�h��
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
			slash->SetColor(Red);
			player->Damage(m_Power);
			m_hit = true;
		}
	}
	else if (m_cnt == m_Startup + m_ActiveFrames)
	{
		if (m_DubleAttackRate >= rand() % 100&&m_DoubleAttack==false)
		{
			m_GameObject->SetAnimName2("Attack3");
			m_GameObject->SetAnimSpeed(0.5);
			m_Startup = 32;
			m_ActiveFrames = 10;
			m_Recovery = 42;
			m_cnt = 0;
			m_hit = false;
			m_DoubleAttack=true;
		}
	}
	//else if (cnt < Startup +m_ActiveFrames + Recovery)         //�U����̍d�����ԁB���̊Ԃ̈��̃^�C�~���O�ł��������ǍU���{�^���������ƘA���ɂȂ�
	//{
	//	if (Input::GetController(Input::R1, Input::PRESSED, 15) != -1 &&
	//		player->GetST() > 0.0f &&
	//		cnt > Startup +m_ActiveFrames + (Recovery / 3))     //�d�����n�܂��Ă����͘A���Ɉڍs�ł��Ȃ�
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
#if _DEBUG
	ImGui::Begin("EnemyAttaState");
	ImGui::SliderInt("Frame", &m_Recovery, 0, 50);
	ImGui::End();
#endif
}