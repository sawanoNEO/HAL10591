#pragma once

#include "../GameObject/gameObject.h"
#include <string>
#include <unordered_map>

class Rigidbody;

class Enemy : public GameObject
{
protected:
	static std::vector<Enemy*> m_Enemyes;//���݂̃V�[���ŉ��̂̓G�����݂��Ă��邩
	const char* m_FileDataPath;//�p�����[�^��񂪓����Ă���csv�̃p�X
	int m_Number=0;//�C���X�^���X���ɐU�蕪�����Ă���ID�̂悤�Ȃ��́A��������ŗ��p����
	class AnimationModel* m_Model;
	std::unordered_map< const char*, class Audio*> m_SE{};
	float m_Frame1;//�Đ����̃A�j���[�V�����̃t���[����
	float m_Frame2;//�Đ����̃A�j���[�V�����̃t���[����
	float m_BlendRate;

	float MaxHP;    //�ő�̗� 
	float HP;       //���ݑ̗�
	bool hit;       //���̍U���ŕ�����U����������������ɂȂ�Ȃ����߂̃t���O
	float groundHeight; //���̍���
	Rigidbody* rb;
	bool Phit = false;
	float m_EyeSight=15.0f;//�ǂ̂��炢�̉����܂Ō����邩

	std::string m_Animname1 = "Idle"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(1)
	std::string m_Animname2 = "Walk"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(2)
	float m_AnimSpeed = 1.0f;//�A�j���[�V�����̍Đ����x(�{��)

	class EnemyHP* m_HP;

public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit()override;
	bool Damage(float);
	void HitReset();

	void SetAnimName2(const char*) override;//�A�j���[�V�����؂�ւ�
	void SetAnimSpeed(float _speed)override { m_AnimSpeed = _speed; }//�A�j���[�V�����̍Đ����x�̐ݒ�

	float GetEyeSight() { return m_EyeSight; }
	static std::vector<Enemy*> GetEnemys();

	void PlaySE(const char* _SEname)override;
	void PlaySE(const char* _SEname, bool _loop)override;
	void StopSE(const char* _SEname)override;
};