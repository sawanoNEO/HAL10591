#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"

#include "../GameObject/camera.h"
#include "../GameObject/WallDodgeCamera.h"
#include "../GameObject/field.h"
#include "../GameObject/player.h"
#include "../GameObject/box.h"
#include "../GameObject/cylinder.h"

#include "../Scene/game.h"
#include "../System/input.h"
#include "../Component/audio.h"
#include "../GameObject/Polygon2D.h"

#include "../System/manager.h"
#include "../System/utility.h"
#include "../GameObject/transition.h"

#include "../Scene/result.h"

#include "../GameObject/goal.h""
#include "../GameObject/bullet.h"
#include "../GameObject/enemy.h"
#include "../GameObject/battery.h"
#include "../GameObject/sky.h"
#include "../GameObject/score.h"
#include "../GameObject/ST.h"
#include "../GameObject/HP.h"
#include "../GameObject/ColiderLooker.h"
#include "../GameObject/explosion.h"
#include "../GameObject/AttackObject.h"

#include"../GameObject/Effect/Slash.h"

#include "../ImGui/imguimanager.h"

//��������
#include "../Component/Obb.h"

#include <fstream>
using namespace DirectX::SimpleMath;

// �Q�[����������
void Game::Init()
{
	Bullet::Load();
	Explosion::Load();
	Slash::Load();

	AddGameObject<Camera>(0);
	AddGameObject<WallDodgeCamera>(1);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1);
	//AddGameObject<Player>(1);
	//AddGameObject<Score>(3);
	AddGameObject<ST>(3);
	AddGameObject<HP>(3);
	//AddGameObject<AttackObject>(1);

	// �G�ǉ�
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(5.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(10.0f, 0.0f, 5.0f));
	//AddGameObject<Enemy>(1)->SetPosition(Vector3(0.0f, 0.0f,10.0f));



	//// �`�F�b�N����
	//{
	//	Box* box = AddGameObject<Box>(1);
	//	box->SetPosition(Vector3(-5.0f, 0.0f, 5.0f));
	//	box->SetScale(Vector3(3.0f, 1.0f, 3.0f));
	//}

	//// �`�F�b�N����
	//{
	//	Box* box = AddGameObject<Box>(1);
	//	box->SetPosition(Vector3(-11.0f, 0.0f, 5.0f));
	//	box->SetScale(Vector3(3.0f, 2.0f, 3.0f));
	//}

	//// �`�F�b�N����
	//{
	//	Box* box = AddGameObject<Box>(1);
	//	box->SetPosition(Vector3(-11.0f, 0.0f, 11.0f));
	//	box->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	//}
	
	// �`�F�b�N����
	{
		Box* box = AddGameObject<Box>(1);
		box->SetPosition(Vector3(15.0f, 0.0f, 0.0f));
		box->SetScale(Vector3(5.0f, 1.0f, 5.0f));
	}

	//// �`�F�b�N����
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(-11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

	//// �`�F�b�N����
	//{
	//	Box* box = AddGameObject<Box>(1);
	//	box->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
	//	box->SetScale(Vector3(9.0f, 3.0f, 1.0f));
	//}

	//// �`�F�b�N����
	//{
	//	Cylinder* cylinder = AddGameObject<Cylinder>(1);
	//	cylinder->SetPosition(Vector3(11.0f, 0.0f, 20.0f));
	//	cylinder->SetScale(Vector3(3.0f, 6.0f, 3.0f));
	//}

	// �`�F�b�N����
	{
		Goal* goal = AddGameObject<Goal>(1);
		goal->SetPosition(Vector3(0.0f, 10.5f, -10.0f));
		goal->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	}

	//AddGameObject<Battery>(1)->SetPosition(Vector3(20.0f, 0.0f, 5.0f));

	// ��ʑJ�ڃI�u�W�F�N�g��o�^
	m_Transition = AddGameObject<Transition>(3);
	m_Transition->FadeIn();
}

// �Q�[���I������
void Game::Uninit()
{
}

// �Q�[���X�V����
void Game::Update()
{
	// �t�F�[�h�C�����I�����Ă��邩�H	
//	if (m_Transition->GetState() == Transition::State::Stop) {
//		if (Input::GetKeyTrigger(VK_RETURN))
//		{
//			m_Transition->FadeOut();
//		}
//	}

	// �S�[�����Ă��Ȃ��̂ł����
	if (!m_Goal)
	{
		Goal* goal = GetGameObject<Goal>();
		Enemy* enemy = GetGameObject<Enemy>();

		// �S�[�������ۂɃS�[���I�u�W�F�N�g�͍폜�����
		if (goal == nullptr || enemy == nullptr)
		{
			m_Goal = true;

			// �Q�b��ɃX���b�h�𐶐����ăt�F�[�h�A�E�g�J�n
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}

	if (Input::GetController(Input::b, Input::PRESSED))
	{
		Slash* slash = AddGameObject<Slash>(2);
		slash ->SetPosition(Vector3(-4.0f, 3.0f, 0.0f));
		slash->SetColor(White);
	}
	
	if (Input::GetController(Input::y, Input::PRESSED))
	{
		Slash* slash = AddGameObject<Slash>(2);
		slash ->SetPosition(Vector3(-4.0f, 3.0f, 0.0f));
		slash->SetColor(Red);
	}

	// �t�F�[�h�A�E�g���I�����Ă��邩�H
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Result>();
	}
}

void Game::Draw()
{
	ImGui::Begin("GameScene");
	if (ImGui::Button("SpawnEnemy"))
	{
		Enemy* enemy=AddGameObject<Enemy>(1);
		enemy->SetPosition(Vector3{ 0.0f,0.0f,5.0f });
	}
	ImGui::End();
}
