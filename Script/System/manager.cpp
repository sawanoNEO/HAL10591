#include	"../System/main.h"
#include	"../System/renderer.h"
#include	"../System/manager.h"
#include	"../Component/audio.h"
#include	"../System/input.h"
#include	"../Scene/scene.h"
#include	"../Scene/game.h"
#include	"../Scene/title.h"
#include    "../ImGui/imguimanager.h"

// �V�[���}�l�[�W���������o�ϐ��̎���
Scene* Manager::m_Scene{};					// ���݃V�[��

void Manager::Init(Application* ap)
{
	// �^�C�}�𑜓x���P�~���b�ɂ���
	timeBeginPeriod(1);

	// �����_���[��������
	Renderer::Init(ap);

	//ImGui��Init
	ImGuiManager::ImguiInit(ap);

	// ��������
	Audio::InitMaster();

	// ���͂�������
	Input::Init();

	// Title�������V�[���ɓo�^
	//SetScene<Title>();
	SetScene<Game>();

}

void Manager::Uninit()
{
	//ImGui��UnInit
	ImGuiManager::ImguiUnInit();

	m_Scene->UninitBase();
	delete m_Scene;

	// �R�c���f����S�ĉ��
	ModelRenderer::UnloadAll();

	// �C���v�b�g�I��
	Input::Uninit();

	// �I�[�f�B�I�I��
	Audio::UninitMaster();

	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);

}

void Manager::Update(uint64_t d)
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();
	ImGuiManager::Begin();

	m_Scene->DrawBase();

	ImGuiManager::End();
	Renderer::End();
}
