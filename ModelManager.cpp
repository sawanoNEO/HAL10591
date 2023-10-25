#include "ModelManager.h"

#include "scene.h"
#include "manager.h"
#include "player.h"
#include "animationModel.h"
#include "input.h"
#include"imguimanager.h"

void ModelManager::Init()
{
	animationmodel = m_GameObject->GetComponent<AnimationModel>();
	animationmodel->Load("asset\\model\\Standing Taunt Battlecry.fbx");				// animation ok
	//animationmodel->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Idle");
	//animationmodel->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Run");
	animationmodel->LoadAnimation("asset\\model\\Player\\Sword And Shield Idle.fbx", "Idle");
	animationmodel->LoadAnimation("asset\\model\\Player\\Sword And Shield Walk.fbx", "Walk");

	Scene* scene = Manager::GetScene();

	player = scene->GetGameObject<Player>();
	
}

void ModelManager::Update()
{
	animationmodel->Update("Idle", m_frame, "Walk", m_frame, m_BlendRate);
	if (Input::GetKeyPress('W'))
	{
		m_BlendRate += 0.1f;
		m_frame++;

	}
	else if (Input::GetKeyPress('S'))
	{
		m_BlendRate += 0.1f;
		m_frame++;
	}
	else
	{
		m_BlendRate -= 0.1f;
		m_frame++;
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

	m_frame++;
}

void ModelManager::Draw()
{
	ImGui::Begin("MM");
	ImGui::SliderFloat("Blend", &m_BlendRate, 0.0, 1.0);
	ImGui::End();
}
