#include	"../System/main.h"
#include	"../System/renderer.h"
#include	"../System/manager.h"
#include	"../Component/audio.h"
#include	"../System/input.h"
#include	"../Scene/scene.h"
#include	"../Scene/game.h"
#include	"../Scene/title.h"
#include    "../ImGui/imguimanager.h"

// シーンマネージャ内メンバ変数の実体
Scene* Manager::m_Scene{};					// 現在シーン

void Manager::Init(Application* ap)
{
	// タイマ解像度を１ミリ秒にする
	timeBeginPeriod(1);

	// レンダラーを初期化
	Renderer::Init(ap);

	//ImGuiのInit
	ImGuiManager::ImguiInit(ap);

	// 音初期化
	Audio::InitMaster();

	// 入力を初期化
	Input::Init();

	// Titleを初期シーンに登録
	//SetScene<Title>();
	SetScene<Game>();

}

void Manager::Uninit()
{
	//ImGuiのUnInit
	ImGuiManager::ImguiUnInit();

	m_Scene->UninitBase();
	delete m_Scene;

	// ３Ｄモデルを全て解放
	ModelRenderer::UnloadAll();

	// インプット終了
	Input::Uninit();

	// オーディオ終了
	Audio::UninitMaster();

	// タイマ解像度を元に戻す
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
