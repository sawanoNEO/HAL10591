#include "../GameObject/HP.h"
#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../GameObject/score.h"
#include "../GameObject/camera.h"
#include "../Component/shader.h"
#include "../GameObject/player.h"
#include "../Component/sprite.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void HP::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (player)
	{
		maxHP = player->GetMaxHP();

		shader = AddComponent<Shader>();
		sprite = AddComponent<Sprite>();

		posX = 20;
		posY = 60;

		//�V�F�[�_�[�ǂݍ���
		shader->Load("shader\\unlitTextureVS.cso", "shader\\PS_HPguage.cso");
		sprite->Init(posX, posY, maxHP / 3.5f, 20, "asset\\texture\\grass.jpg");
		damage = false;
	}
}

void HP::Uninit()
{

}

void HP::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	//HPGauge hpgauge{};

	if (player) //�v���C���[�I�u�W�F�N�g��������Ȃ������ꍇ�������X�L�b�v����
	{
		//�v���C���[��HP�f�[�^���擾����
		if (!damage)//�_���[�W���󂯂Ă��Ȃ�������
		{
			pastHP = currentHP; //��O�̃t���[����HP���擾����
		}
		else if (damage&&cnt<30)
		{
			cnt++;
		}
		else if (cnt >= 30)
		{
			damage = false;
		}
		m_damegePoint = player->GetHP();
		if (m_damegePoint>currentHP)
		{
			float gap = m_damegePoint - currentHP;
			currentHP += gap/10;//HP�̃f�[�^���X�V����
		}
		else if (m_damegePoint<currentHP)
		{
			float gap = currentHP - m_damegePoint;
			currentHP -= gap/10;//HP�̃f�[�^���X�V����
		}
		//if (pastHP > currentHP) //�O�̃t���[����HP��茻�݂�HP��������Ă�����_���[�W���󂯂��Ƃ������ƂɂȂ�
		//{
		//	damage = true;
		//}

		hpgauge.MaxHP = (maxHP / 100)/3.5f;
		hpgauge.currentHP = (currentHP / 1000);
		hpgauge.pastHP = (pastHP / 1000);
		//Renderer::SetHPGauge(hpgauge);
	}
}

void HP::Draw()
{
	ImGui::Begin("HP");
	ImGui::Text("pastHP=%f", &pastHP);
	ImGui::Text("currentHP=%f", &currentHP);
	if (ImGui::Button("SwitchON!!"))
	{
		damage != damage;
	}
	ImGui::Text("MaxHP%f", (maxHP / 100) / 3.5f);
	ImGui::Text("CurrentHP%f", currentHP / 1000);
	ImGui::Text("pastHP%f", pastHP / 1000);
	ImGui::End();
	Renderer::SetHPGauge(hpgauge);

}
