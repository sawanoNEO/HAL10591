#include "ST.h"
#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "score.h"
#include "../GameObject/camera.h"
#include "../Component/shader.h"
#include "../GameObject/player.h"
#include "../Component/sprite.h"

using namespace DirectX::SimpleMath;

void ST::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (player)
	{
		float maxST = player->GetMaxST();
	shader = AddComponent<Shader>();
	sprite = AddComponent<Sprite>();

	posX = 20;
	posY = 80;

	//シェーダー読み込み
	shader->Load("shader\\unlitTextureVS.cso", "shader\\PS_STgauge.cso");
	sprite->Init(posX, posY, maxST * 2.0, 20, "asset\\texture\\grass.jpg");

	}
}

void ST::Uninit()
{

}

void ST::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Gauge gauge{};

	if (player)
	{
		float maxST = player->GetMaxST();
		float currentST = player->GetST();
		gauge.MaxST = maxST / 100;
		gauge.currentST = currentST / 100;
		Renderer::SetGauge(gauge);
	}
}
