#include "main.h"
#include "manager.h"
#include "renderer.h"
//#include "input.h"
//#include "scene.h"
#include "modelRenderer.h"
#include "box.h"
//#include "shadow.h"
#include "shader.h"
#include "Colider.h"
#include <SimpleMath.h>
#include "Rigidbody.h"
#include "Move.h"

#include "player.h"
#include "imguimanager.h"

using namespace DirectX::SimpleMath;

void Box::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

//	AddComponent<Shadow>()->SetSize(8.0f);
	AddComponent<Colider>();
	float a = GetScale().y / 3;     //当たり判定のサイズ調整に使用する
	if (a < 1)
	{
		a = 0;
	}
	float YSize = 5.0f * -(a * 0.1);
	Vector3 scale = GetScale();
	GetComponent<Colider>()->Init(WALL, Vector3{ 2.0f, YSize , 2.0f});
	GetComponent<Colider>()->SetAABB(m_Position,fabs(scale.x),
		fabs(YSize * scale.y ),
		fabs(scale.z));
}

void Box::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();


	//////////////////////////////////各方向からの当たり判定を取得する。なにかに当たるとそのオブジェクトの情報を取得する．
	if (player)
	{
		Colider col = GetComponent<Colider>()->CollisionAABB(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());
		Colider colright = GetComponent<Colider>()->CollisionAABBRight(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());
		Colider colleft = GetComponent<Colider>()->CollisionAABBLeft(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());
		Colider coltop = GetComponent<Colider>()->CollisionAABBTop(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());
		Colider colbottom = GetComponent<Colider>()->CollisionAABBBottom(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());
		Colider colhead = GetComponent<Colider>()->CollisionAABBHead(GetComponent<Colider>()->GetAABB(), player->GetComponent<Colider>());

		Vector3 oldpos = player->GetComponent<Move>()->GetOldPos();    //プレイヤーの前のフレームのposition
		Vector3 pos = player->GetPosition();                           //プレイヤーの現在位置
		Vector3 vel = player->GetComponent<Rigidbody>()->GetVelocity();//プレイヤーの速度を取得


		if (colright.GetTug() == PLAYER)
		{
			if (vel.x < 0)
			{
				pos.x = oldpos.x;
				vel.x = 0;
			}
			player->SetPosition(pos);
			player->GetComponent<Rigidbody>()->SetVelocity(vel);
		}
		else if (colleft.GetTug() == PLAYER)
		{
			if (vel.x > 0)
			{
				pos.x = oldpos.x;
				vel.x = 0;
			}
			player->SetPosition(pos);
			player->GetComponent<Rigidbody>()->SetVelocity(vel);
		}
		if (coltop.GetTug() == PLAYER)
		{
			if (vel.z < 0)
			{
				pos.z = oldpos.z;
				vel.z = 0;
			}
			player->SetPosition(pos);
			player->GetComponent<Rigidbody>()->SetVelocity(vel);
		}
		if (colbottom.GetTug() == PLAYER)
		{
			if (vel.z > 0)
			{
				pos.z = oldpos.z;
				vel.z = 0;
			}
			player->SetPosition(pos);
			player->GetComponent<Rigidbody>()->SetVelocity(vel);
		}
	}
}

void Box::Draw()
{
	AABB2 c=GetComponent<Colider>()->GetAABB();
	ImGui::Begin("Box");
	//ImGui::Text("%i", d);
	ImGui::Text("%f", c.max.x);
	ImGui::Text("%f", c.min.x);
	ImGui::Text("%f", c.max.z);
	ImGui::Text("%f\n\n", c.min.z);
	ImGui::End();
}
