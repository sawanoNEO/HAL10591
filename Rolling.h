#pragma once
#include "component.h"
#include <SimpleMath.h>

class Rolling : public Component
{
private:
	int startup;   //ローリング(回避)の無敵時間が発生するまでの時間(フレーム)
	int invincible; //無敵時間
	int recovery;   //ローリングの後に行動可能になるまでの時間(フレーム)
	int reception;  //ダッシュボタンを押してから離すまでの間に回避が成立する猶予フレーム
	int cnt;
	int receptioncnt; //猶予フレームを判断するための変数
	DirectX::SimpleMath::Vector3 Rolvec;   //ローリング&バックステップの方向を決める変数

public:
	using Component::Component;
	void Init()override;
	void Update()override;
};

