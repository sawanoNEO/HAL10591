#pragma once
#include"../Component/component.h"
#include<list>

class AnimationModel;

class AnimationManager : public Component
{
private:
	int m_frame;        //再生中のアニメーションのフレーム数
	float	m_blendRate;              //アニメーションブレンドのブレンド率
	AnimationModel* m_animationmodel; //モデルとアニメーションのデータ
	char* m_currentAnimationName;      //現在再生しているアニメーション
	char* m_lastAnimationName;         //前回再生していたアニメーション
	std::list<const char*> animationlist;    //読み込んだアニメーション

public:
	using Component::Component;
	void Init();
	void Update()override;
	void Draw()override;
	void AddAnimation(const char* FileName, const char* Name);

};

