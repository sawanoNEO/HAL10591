#pragma once

#include "gameObject.h"

class Rigidbody;

class Enemy : public GameObject
{
private:
	enum State
	{
		NONE,
		NORMAL,    //�ʏ�̏�ԁB��@���
		BATTLE,    //�v���C���[�������čU�����Ă�����
	};
	State state;
	float MaxHP;    //�ő�̗� 
	float HP;       //���ݑ̗�
	bool hit;       //���̍U���ŕ�����U����������������ɂȂ�Ȃ����߂̃t���O
	float groundHeight; //���̍���
	Rigidbody* rb;
	bool Phit=false;
	float eyesight;//�ǂ̂��炢�̉����܂Ō����邩
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Damage(float);
	void HitReset();

	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//����͈̓`�F�b�N�֐��B
};