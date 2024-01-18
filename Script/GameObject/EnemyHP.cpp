#include "EnemyHP.h"

#include"camera.h"


#include "../Component/shader.h"
#include "../Component/sprite.h"

#include "../System/modelRenderer.h"

#include "../System/renderer.h"
#include "../System/main.h"
#include "../System/manager.h"

#include "../Scene/scene.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void EnemyHP::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\PS_HPguage.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\BillBoard.obj");


	m_Position = Vector3{ 0.0f,200.0f,0.0f };
	m_Scale = Vector3{ 100.0f,10.0f,10.1f };
	maxHP = 100;
}



void EnemyHP::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	//HPGauge hpgauge{};
	Vector3 campos = camera->GetPosition();
	Vector3 forward = m_Position - campos;
	forward.Normalize();

	//m_Rotation.y = atan2(forward.x, forward.z);

	//親のHPデータを取得する
	if (!damage)//ダメージを受けていなかったら
	{
		pastHP = currentHP; //一つ前のフレームのHPを取得する
	}
	else if (damage && cnt < 30)
	{
		cnt++;
	}
	else if (cnt >= 30)
	{
		damage = false;
	}
	//m_damegePoint = player->GetHP();
	if (m_damegePoint > currentHP)
	{
		float gap = m_damegePoint - currentHP;
		currentHP += gap / 10;//HPのデータを更新する
	}
	else if (m_damegePoint < currentHP)
	{
		float gap = currentHP - m_damegePoint;
		currentHP -= gap / 10;//HPのデータを更新する
	}
	//if (pastHP > currentHP) //前のフレームのHPより現在のHPが下回っていたらダメージを受けたということになる
	//{
	//	damage = true;
	//}

	hpgauge.MaxHP = maxHP / 100;
	hpgauge.currentHP = currentHP / 100*2.5;
	hpgauge.pastHP = pastHP / 100*2.5;
	//Renderer::SetHPGauge(hpgauge);
}



void EnemyHP::Draw()
{
	ImGui::Begin("EnemyHP");
	ImGui::Text("pos%f%f%f",m_Position.x,m_Position.y,m_Position.z);
	ImGui::Text("MaxHP%f",maxHP / 100);
	ImGui::Text("currentHP%f",currentHP / 100);
	ImGui::Text("currentHP%f",pastHP / 100);
	ImGui::End();
	Renderer::SetHPGauge(hpgauge);

}

void EnemyHP::deliverParamater(float _hp)
{
	m_damegePoint = _hp;
}
