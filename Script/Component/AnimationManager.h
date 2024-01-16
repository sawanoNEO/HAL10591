#pragma once
#include"../Component/component.h"
#include<list>

class AnimationModel;

class AnimationManager : public Component
{
private:
	int m_frame;        //�Đ����̃A�j���[�V�����̃t���[����
	float	m_blendRate;              //�A�j���[�V�����u�����h�̃u�����h��
	AnimationModel* m_animationmodel; //���f���ƃA�j���[�V�����̃f�[�^
	char* m_currentAnimationName;      //���ݍĐ����Ă���A�j���[�V����
	char* m_lastAnimationName;         //�O��Đ����Ă����A�j���[�V����
	std::list<const char*> animationlist;    //�ǂݍ��񂾃A�j���[�V����

public:
	using Component::Component;
	void Init();
	void Update()override;
	void Draw()override;
	void AddAnimation(const char* FileName, const char* Name);

};

