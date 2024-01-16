#pragma once
#include "../Component/component.h"


class AnimationModel;
class Player;

class ModelManager : public Component
{
private:
	int m_frame;        //再生中のアニメーションのフレーム数
	float	m_blendRate;              //アニメーションブレンドのブレンド率
	AnimationModel* m_animationmodel; //モデルとアニメーションのデータ
	char* m_currentAnimationName;      //現在再生しているアニメーション
	char* m_lastAnimationName;         //前回再生していたアニメーション
	Player* player;
	
public:
	using Component::Component;
	void Init();
	void Update()override;
	void Draw()override;
	void AddAnimation(const char* FileName, const char* Name);

	//セッター
	void SetcurrentAnimation(const char* Name);
	void LoadAnimation(const char* FileName, const char* Name);
};

