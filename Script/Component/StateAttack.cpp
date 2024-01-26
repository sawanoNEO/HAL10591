#include "StateAttack.h"
#include "StateNone.h"
#include "Colider.h"
#include "Rigidbody.h"

#include "../Scene/scene.h"

#include "../System/manager.h"

#include "../GameObject/gameObject.h"
#include "../GameObject/player.h"
#include "../GameObject/enemy.h"
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
	Maxcombo = 2;
	combo = 0;
	Startup = 22;
	ActiveFrames = 5;
	Recovery = 23;
	cnt = 0;
	Power = 45;
	STconsumption = 17.0;
}

void StateAttack::Enter()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	player->SetAnimName2("Attack");
	cnt = 0;
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
	std::vector<Colider*> col;//�I�u�W�F�N�g�̓����蔻����擾����
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	bool hit = false;
	Rigidbody* rb = player->GetComponent<Rigidbody>();

	AttackObj->SetPosition(player->GetPosition()+forward*3);

	col.resize(enemys.size());
	for (int i = 0; i < enemys.size(); i++)
	{
		col[i] = enemys[i]->GetComponent<Colider>();
	}

	if (cnt < Startup)
	{

		STconsumption = 17.0f;

		for (const auto enemy : enemys)
		{
			enemy->HitReset();
		}
	}
	else if (cnt < Startup + ActiveFrames)/////�U�����肪�o�Ă��鎞�ԁB���������B
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));


		if (cnt == Startup)
		{
			slash = scene->AddGameObject<Slash>(1);
			slash->SetColor(White);
			Vector3 pos = player->GetPosition() + player->GetForward() * 6;
			pos.y += 2.0;
			//slash->SetPosition(player->GetPosition() + player->GetForward() * 5);
			slash->SetPosition(pos);
			Vector3 rot = { 0.0,0.0,0.0 };
			rot.z += cos(90 * 3.14 / 180);
			slash->SetRotation(rot);
			player->STUse(STconsumption);
			Vector3 vec = player->GetForward() * 100.0f;
			if (Input::GetStickState())
			{
				Vector3 vec = player->GetpromissDirection() * 100.0f;
			}
			rb->AddForce(vec, ForceMode::Impulse);
		}

		if (enemys.size() == 0)
		{
			return;
		}
		for (int i = 0; i < enemys.size(); i++)
		{
			Colider* hitobj = col[i]->CollisionAABB(AttackObj->GetComponent<Colider>()->GetAABB(), col[i]);
			if (hitobj != nullptr && hitobj->GetTug() == ENEMY)
			{
				if (enemys[i] && !hit)
				{
					Vector3 enemypos = enemys[i]->GetPosition();
					enemypos.y += 1.0f;
					enemys[i]->Damage(Power);
					slash->SetColor(Red);
					//slash->SetPosition(enemypos);
				}
			}
		}
	}
	else if (cnt < Startup + ActiveFrames + Recovery)         //�U����̍d�����ԁB���̊Ԃ̈��̃^�C�~���O�ł��������ǍU���{�^���������ƘA���ɂȂ�
	{
		if (Input::GetController(Input::R1, Input::PRESSED,15)!=-1 &&
			player->GetST() > 0.0f &&
			cnt > Startup + ActiveFrames + (Recovery / 3))     //�d�����n�܂��Ă����͘A���Ɉڍs�ł��Ȃ�
		{
			cnt = Startup - 10;
			player->SetFrame1(0);
			//player->STUse(17.0f);
		}
	}
	
	cnt++;
}

void StateAttack::StateChange()
{
	if (cnt > Startup + ActiveFrames + Recovery)
	{
		m_GameObject->GetComponent<StateMachine>()->changeState(m_GameObject->GetComponent<StateNone>());
	}
}

void StateAttack::Draw()
{
	ImGui::Begin("StateAttack");
	ImGui::SliderInt("Recovery", &Recovery, -20, 20);
	ImGui::End();
}
