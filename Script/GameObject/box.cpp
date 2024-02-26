#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/modelRenderer.h"
#include "../GameObject/box.h"
#include "../Component/shader.h"
#include "../Component/Colider.h"
#include <SimpleMath.h>
#include "../Component/Rigidbody.h"

#include "../GameObject/player.h"
#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void Box::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\box.obj");

	AddComponent<Colider>();
	float a = GetScale().y / 3;     //�����蔻��̃T�C�Y�����Ɏg�p����
	if (a < 1)
	{
		a = 0;
	}
	Vector3 max = GetComponent<ModelRenderer>()->GetMaxPos();
	Vector3 min = GetComponent<ModelRenderer>()->GetMinPos();
	Vector3 scale;
	scale.x = fabsf(max.x) + fabsf(min.x);
	scale.y = fabsf(max.y) + fabsf(min.y);
	scale.z = fabsf(max.z) + fabsf(min.z);
	float YSize = 5.0f * -(a * 0.1);
	GetComponent<Colider>()->Init(WALL, scale);
	GetComponent<Colider>()->SetAABB(m_Position,fabs(m_Scale.x),
		fabs(YSize * m_Scale.y ),
		fabs(m_Scale.z));
}

void Box::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	std::vector<GameObject*> gameobjects = scene->GetAllObjects();//�V�[�����̃I�u�W�F�N�g���擾
	std::vector<GameObject*>coliderobjects;  //Colider�������Ă���GameObject
	for (auto object : gameobjects)
	{
		if (object->GetComponent<Colider>()&& object->GetComponent<Rigidbody>())
		{
			coliderobjects.push_back(object);
		}
	}

	//////////////////////////////////�e��������̓����蔻����擾����B�Ȃɂ��ɓ�����Ƃ��̃I�u�W�F�N�g�̏����擾����D
	for (auto objects:coliderobjects)
	{
		Colider* colider = objects->GetComponent<Colider>();
		Colider* colright = GetComponent<Colider>()->CollisionAABBRight(GetComponent<Colider>()->GetAABB(), colider);
		Colider* colleft = GetComponent<Colider>()->CollisionAABBLeft(GetComponent<Colider>()->GetAABB(), colider);
		Colider* coltop = GetComponent<Colider>()->CollisionAABBTop(GetComponent<Colider>()->GetAABB(), colider);
		Colider* colbottom = GetComponent<Colider>()->CollisionAABBBottom(GetComponent<Colider>()->GetAABB(), colider);
		Colider* colhead = GetComponent<Colider>()->CollisionAABBHead(GetComponent<Colider>()->GetAABB(), colider);


		Vector3 oldpos = objects->GetOldPosition();    //�I�u�W�F�N�g�̑O�̃t���[����position
		Vector3 pos = objects->GetPosition();                           //�I�u�W�F�N�g�̌��݈ʒu
		Vector3 vel = objects->GetComponent<Rigidbody>()->GetVelocity();//�I�u�W�F�N�g�̑��x���擾

		Colider* hitcheck = GetComponent<Colider>()->CollisionAABB(GetComponent<Colider>()->GetAABB(), colider);

		if (hitcheck != nullptr)
		{
			//if (colright->GetTug() != DEFAULT)
			if (colright)
			{
				if (vel.x < 0)
				{
					pos.x = oldpos.x;
					vel.x = 0;
				}
				objects->SetPosition(pos);
				objects->GetComponent<Rigidbody>()->SetVelocity(vel);
			}
			//else if (colleft->GetTug() != DEFAULT)
			else if (colleft)
			{
				if (vel.x > 0)
				{
					pos.x = oldpos.x;
					vel.x = 0;
				}
				objects->SetPosition(pos);
				objects->GetComponent<Rigidbody>()->SetVelocity(vel);
			}
			//if (coltop->GetTug() != DEFAULT)
			if (coltop)
			{
				if (vel.z < 0)
				{
					pos.z = oldpos.z;
					vel.z = 0;
				}
				objects->SetPosition(pos);
				objects->GetComponent<Rigidbody>()->SetVelocity(vel);
			}
			//if (colbottom->GetTug() != DEFAULT)
			if (colbottom)
			{
				if (vel.z > 0)
				{
					pos.z = oldpos.z;
					vel.z = 0;
				}
				objects->SetPosition(pos);
				objects->GetComponent<Rigidbody>()->SetVelocity(vel);
			}
		}
	}
}

void Box::Draw()
{
	AABB c=GetComponent<Colider>()->GetAABB();

	ImGui::Begin("Box");
	//ImGui::Text("%i", d);
	ImGui::Text("%f", c.max.x);
	ImGui::Text("%f", c.min.x);
	ImGui::Text("%f", c.max.z);
	ImGui::Text("%f\n\n", c.min.z);
	ImGui::End();

}
