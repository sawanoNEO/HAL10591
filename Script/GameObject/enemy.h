#pragma once

#include "../GameObject/gameObject.h"

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
	class AnimationModel* m_Model;
	float m_Frame1;//�Đ����̃A�j���[�V�����̃t���[����
	float m_Frame2;//�Đ����̃A�j���[�V�����̃t���[����
	float m_BlendRate;

	float MaxHP;    //�ő�̗� 
	float HP;       //���ݑ̗�
	bool hit;       //���̍U���ŕ�����U����������������ɂȂ�Ȃ����߂̃t���O
	float groundHeight; //���̍���
	Rigidbody* rb;
	bool Phit = false;
	float eyesight;//�ǂ̂��炢�̉����܂Ō����邩

	std::string m_Animname1 = "Idle"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(1)
	std::string m_Animname2 = "Walk"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(2)

	class EnemyHP* m_HP;



public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Damage(float);
	void HitReset();

	void SetAnimName2(const char*);//�A�j���[�V�����؂�ւ�

	bool SearchPlayer(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 m_pos, float fov, float length);//����͈̓`�F�b�N�֐��B
};