#include "AnimationManager.h"
#include "animationModel.h"
#include"../GameObject/gameObject.h"
#include "../System/input.h"
#include"../ImGui/imguimanager.h"

void AnimationManager::Init()
{
	m_animationmodel = m_GameObject->GetComponent<AnimationModel>();
	m_animationmodel->Load("asset\\model\\Standing Taunt Battlecry.fbx");
	if (!m_animationmodel)
	{
		assert("AnimationModel‚ª‚ ‚è‚Ü‚¹‚ñ");
	}
}

void AnimationManager::Update()
{
	m_animationmodel->Update("Idle", m_frame, "Walk", m_frame, m_blendRate);
	m_frame++;
	m_blendRate = 0.0f;
}

void AnimationManager::Draw()
{
	ImGui::Begin("AnimationManager");
	for (auto itr = animationlist.begin(); itr != animationlist.end(); ++itr)
	{
		ImGui::Text("%s", *itr);
	}
	ImGui::End();
}

void AnimationManager::AddAnimation(const char* FileName, const char* Name)
{
	m_animationmodel->LoadAnimation(FileName, Name);
	animationlist.push_back(Name);
}

//void AnimationManager::SetcurrentAnimation(const char* Name)
//{
//}
