#include "Sword.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"

#include "manager.h"
//#include "Colider.h"

#include "Box.h"
#include <vector>
#include "imguimanager.h"

#include"Rigidbody.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Sword::Init()
{
	Maxcombo = 2;
	combo = 0;
	Startup = 32;
	ActiveFrames = 5;
	Recovery = 35;
	cnt = 0;
	Power = 45;
	STconsumption = 17.0;
}

void Sword::Update()
{
	////�����ɕK�v�ȏ����擾
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	Vector3 camforward = camera->GetCamForward();
	Vector3 camside = camera->GetCamSide();
	Vector3 pos = player->GetPosition();
	Vector3 forward = player->GetForward();
	PLAYERSTATE state = player->GetPstate();
	Colider* Pcol = player->Getcolattack();
	Vector3 position = pos;
	position += 0.2 * forward;
	//position.x = pos.x * (forward.x * 1.5);
	//position.z = pos.z * (forward.z * 1.5);
	Pcol->SetAABB(position, 2.0f, 2.0f, 2.0f);//�U������ݒ�
	ab = Pcol->GetAABB();
	std::vector<Enemy*> gameobject=scene->GetGameObjects<Enemy>();//go��GameObject�̈�
	std::vector<Colider*> col;//�I�u�W�F�N�g�̓����蔻����擾���邽�߂̔z��
	Rigidbody* rb = player->GetComponent<Rigidbody>();
	bool hitCheck = false;

	////�����܂�

	col.resize(gameobject.size());
	for (int i = 0; i < gameobject.size(); i++)
	{
		col[i] = gameobject[i]->GetComponent<Colider>();
	}
	
	if (Input::GetController(Input::R2, Input::PRESSED) && state != ATTACK && player->GetST() > 10.0f)//////////���U��
	{
		cnt++;
		state = ATTACK; 
		STconsumption = 25.0f;
		Startup = 40;
		Power = 60;
	}
	if (Input::GetController(Input::R1, Input::PRESSED)&&state!=ATTACK&&player->GetST()>10.0f)////�v���C���[�̏�ԑJ��
	{
		cnt++;
		state = ATTACK;
		STconsumption = 17.0f;

		for (const auto enemy : gameobject)
		{
			enemy->HitReset();
		}
		
	}
	if (state==ATTACK&&cnt < Startup)     ////�U���{�^���������ꂽ�u�Ԃ���U�����肪�o��܂ł̌�(������ƑO�ɓ��ݍ���)
	{
		/*pos += 0.01 * forward;
		player->SetPosition(pos);*/
		cnt++;
	}
	else if (state==ATTACK&&cnt < Startup + ActiveFrames)/////�U�����肪�o�Ă��鎞�ԁB���������B
	{
		player->SetpromissDirection(XMVector3Normalize(camera->VecYRemove(camside) * Input::GetStick(Input::LeftX) + (camera->VecYRemove(camforward) * Input::GetStick(Input::LeftY))));
		if (cnt == Startup)
		{
			player->STUse(STconsumption);
			Vector3 vec = player->GetpromissDirection() * 100.0;
			rb->AddForce(vec, ForceMode::Impulse);
		}
		cnt++;

		if (gameobject.size() == 0)
			return;

		for (int i = 0; i < gameobject.size(); i++)
		{
			Colider hit = col[i]->CollisionAABB(Pcol->GetAABB(), col[i]);
			if ( hit.GetTug() == ENEMY)
			{
				if (gameobject[i] && !hitCheck)
				{
					hitCheck = true;
					gameobject[i]->Damage(Power);
					//go[i]->SetDestroy();
				}
			}
		}
	}
	else if (state == ATTACK && cnt < Startup + ActiveFrames + Recovery)         //�U����̍d�����ԁB���̊Ԃ̈��̃^�C�~���O�ł��������ǍU���{�^���������ƘA���ɂȂ�
	{
		cnt++;
		if (Input::GetController(Input::R1, Input::PRESSED)&&
			player->GetST()>0.0f&&
			cnt>Startup+ActiveFrames+(Recovery/3))     //�d�����n�܂��Ă����͘A���Ɉڍs�ł��Ȃ�
		{
			cnt = Startup - 10;
			//player->STUse(17.0f);
		}
	}
	else if(state == ATTACK)
	{
		cnt = 0;
		state = NONE;
	}

	player->SetPstate(state);//�Ղꂢ��[�̏�Ԃ𔽉f
}

void Sword::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Colider* Pcol = player->Getcolattack();
	AABB2 aabb = Pcol->GetAABB();
	ImGui::Begin("Sword");
	ImGui::Text("MaxX=%f\n",ab.max.x);
	ImGui::Text("MinX=%f\n",ab.min.x);
	ImGui::Text("MaxZ=%f\n",ab.max.z);
	ImGui::Text("MinZ=%f\n",ab.min.z);
	ImGui::End();
}
