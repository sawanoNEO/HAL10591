#pragma once
#include "component.h"
#include <simplemath.h>

class Move : public Component
{
private:

	float Speed;        //基本の移動速度
	float Accel;        //通常の加速度
	float DAccel;       //ダッシュ時の加速度
	float MaxSpeed;     //最大移動速度
	float rotationThreshold = 0.5;//回転するかどうかのズレの許容度
	DirectX::SimpleMath::Vector3 OldPos;
	DirectX::SimpleMath::Vector3 promissDirection{0.0f,0.0f,1.0f};//プレイヤーが振り向く際の最終的に振り向く方向

public:
	using Component::Component;
	void Update();
	void Draw();

	float GetAccel() { return Accel; }
	void SetAccel(float acc) { Accel = acc; }
	float GetDAccel() { return DAccel; }
	void SetDAccel(float acc) { DAccel = acc; }
	DirectX::SimpleMath::Vector3 GetOldPos() { return OldPos; }
};

