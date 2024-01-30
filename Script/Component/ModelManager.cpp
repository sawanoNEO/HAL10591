#include "ModelManager.h"

#include "../Scene/scene.h"
#include "../System/manager.h"
#include "../GameObject/player.h"
#include "../Component/animationModel.h"
#include "../System/input.h"
#include"../ImGui/imguimanager.h"

void ModelManager::Init()
{
	m_animationmodel = m_GameObject->GetComponent<AnimationModel>();
//	m_animationmodel->Load("asset\\model\\Standing Taunt Battlecry.fbx");				// animation ok
//	//m_animationmodel->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Idle");
//	//m_animationmodel->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Run");
//	m_animationmodel->LoadAnimation("asset\\model\\Player\\Sword And Shield Idle.fbx", "Idle");
//	m_animationmodel->LoadAnimation("asset\\model\\Player\\Sword And Shield Walk.fbx", "Walk");
//z
//	Scene* scene = Manager::GetScene();
//
//	player = scene->GetGameObject<Player>();
}

void ModelManager::Update()
{
	m_animationmodel->Update("Idle", m_frame, "Walk", m_frame, m_blendRate);
	if (Input::GetKeyPress('W'))
	{
		m_blendRate += 0.1f;

	}
	else if (Input::GetKeyPress('S'))
	{
		m_blendRate += 0.1f;
	}
	else
	{
		m_blendRate -= 0.1f;
	}

	if (m_blendRate > 1.0f)
		m_blendRate = 1.0f;
	if (m_blendRate < 0.0f)
		m_blendRate = 0.0f;

	m_frame++;
}

void ModelManager::Draw()
{
#if _DEBUG
	ImGui::Begin("MM");
	ImGui::SliderFloat("Blend", &m_blendRate, 0.0, 1.0);
	ImGui::End();
#endif
}

void ModelManager::AddAnimation(const char* FileName, const char* Name)
{
	m_animationmodel->LoadAnimation(FileName, Name);
}

void ModelManager::SetcurrentAnimation(const char* Name)
{
	//m_currentAnimationName = Name;
}

void ModelManager::LoadAnimation(const char* FileName, const char* Name)
{
	m_animationmodel->LoadAnimation(FileName, Name);
}
