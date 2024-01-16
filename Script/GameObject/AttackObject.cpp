#include "AttackObject.h"
#include "player.h"

#include "../Component/Sword.h"
#include "../Component/shader.h"
#include "../Component/Colider.h"
#include "../Component/animationModel.h"

#include "../Scene/scene.h"

#include "../System/modelRenderer.h"
#include "../System/manager.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

void AttackObject::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\unlitTexturePS.cso");
	AddComponent<ModelRenderer>()->Load("asset\\model\\Player\\Sword.obj");
	//m_Position = Vector3{ 1.0f,1.0f,1.0f };
	//AddComponent<Colider>();
	//GetComponent<Colider>()->Init(PLAYER, Vector3{1.0f,1.0f,1.0f});
	m_Scale = Vector3{ 0.2f,0.2f,0.2f };
}

void AttackObject::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Vector3 forward = player->GetForward();

	m_Model = player->GetComponent<AnimationModel>();
	Matrix mat = m_Model->GetBoneMatrix("mixamorig:RightHand");
	SetPosition(Vector3(-mat._41, -mat._42, -mat._43));

	//
	//// シーンからボーンのルートノードを取得
	//aiNode* boneRootNode = m_Model->Getscene()->mRootNode->FindNode("mixamorig:RightHand");

	//// ボーンのトランスフォーム行列を取得
	//aiMatrix4x4 boneTransform = boneRootNode->mTransformation;
 ////   m_Position = player->GetPosition();
	////m_Position += forward * 3;
	//Matrix result;

	//result._11 = boneTransform.a1; result._21 = boneTransform.a2; result._31 = boneTransform.a3; result._41 = boneTransform.a4;
	//result._12 = boneTransform.b1; result._22 = boneTransform.b2; result._32 = boneTransform.b3; result._42 = boneTransform.b4;
	//result._13 = boneTransform.c1; result._23 = boneTransform.c2; result._33 = boneTransform.c3; result._43 = boneTransform.c4;
	//result._14 = boneTransform.d1; result._24 = boneTransform.d2; result._34 = boneTransform.d3; result._44 = boneTransform.d4;

	//Matrix handworld = player->GetMatrix() * result;

	//Matrix matrix = -handworld;
	//m_Position.x = matrix._41;
	//m_Position.y = matrix._42;
	//m_Position.z = matrix._43;

	//m_Rotation.x = matrix._31;
	//m_Rotation.y = matrix._32;
	//m_Rotation.z = matrix._33;

	//// X軸回転
	//m_Rotation.x = atan2f(matrix._32, matrix._33);

	//// Y軸回転
	//m_Rotation.y = atan2f(matrix._31, sqrtf(matrix._32 * matrix._32 + matrix._33 * matrix._33));

	//// Z軸回転
	//m_Rotation.z = atan2f(matrix._21, matrix._11);

	
}

void AttackObject::Draw()
{
	ImGui::Begin("AttackObj");
	//ImGui::SliderFloat("aaaa", &m_Position.y, 0, 50);
	//ImGui::Text("%f%f%f",m_Position.x,m_Position.y,m_Position.z);
	//ImGui::Text("%f%f%f",m_Rotation.x,m_Rotation.y,m_Rotation.z);
	//ImGui::Text("%f%f%f",m_Scale.x,m_Scale.y,m_Scale.z);
	ImGui::SliderFloat("rotationx", &m_Rotation.x, - 3,6);
	ImGui::SliderFloat("rotationy", &m_Rotation.y, - 3,6);
	ImGui::SliderFloat("rotationz", &m_Rotation.z, - 3,6);
	ImGui::SliderFloat("positionx", &m_Position.x, 0,60);
	ImGui::SliderFloat("positiony", &m_Position.y, 0,200);
	ImGui::SliderFloat("positionz", &m_Position.z, 0,60);
	ImGui::Text("Matrix11%f,%f,%f,%f", GetMatrix()._11, GetMatrix()._12, GetMatrix()._13, GetMatrix()._14);
	ImGui::Text("Matrix21%f,%f,%f,%f", GetMatrix()._21, GetMatrix()._22, GetMatrix()._23, GetMatrix()._24);
	ImGui::Text("Matrix31%f,%f,%f,%f", GetMatrix()._31, GetMatrix()._32, GetMatrix()._33, GetMatrix()._34);
	ImGui::Text("Matrix41%f,%f,%f,%f", GetMatrix()._41, GetMatrix()._42, GetMatrix()._43, GetMatrix()._44);
	ImGui::End();
}
