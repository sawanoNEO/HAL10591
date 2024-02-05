#pragma once
#include "gameObject.h"
#include <iostream>
#include <unordered_map>

enum PLAYERSTATE
{
	NONE,
	DASH,
	ATTACK,
	ROLLING,
	JUMP,
	ITEM,
	BACKSTEP,
	DEATH,
	};

	class Colider;

class Player : public GameObject
{
private:
	PLAYERSTATE Pstate = NONE;
	DirectX::SimpleMath::Vector3		m_Velocity{};
	std::unordered_map< const char*,class Audio*> m_SE{};

	class AnimationModel* m_Model;
	AnimationModel* m_ChildBone;
	float m_Frame1;//�Đ����̃A�j���[�V�����̃t���[����
	float m_Frame2;//�Đ����̃A�j���[�V�����̃t���[����
	float m_BlendRate;

	float MaxHP=1000;     //�ő�̗�
	float HP=1000;             //���ݑ̗�
	float groundHeight; //���̍���
	float ST=100;           //�X�^�~�i
	float MaxST = 100;        //�X�^�~�i�ő�l
	float Speed;        //��{�̈ړ����x
	float Accel;        //�ʏ�̉����x
	float DAccel;       //�_�b�V�����̉����x
	float MaxSpeed;     //�ő�ړ����x
	float cnt;          //�e����̃J�E���g
	float m_AnimSpeed = 1.0f;//�A�j���[�V�����̍Đ����x(�{��)
	float m_RecoverST = 0.75f;//�X�^�~�i�̉񕜑��x(1f���Ƃ̉񕜗�)

	bool Action=false;  //�X�^�~�i�������s�������Ă��邩
	bool Wait = false;  //�_�b�V���ŃX�^�~�i���g���؂�������true�ɂȂ�B�X�^�~�i���񕜂�����܂Ń_�b�V���o���Ȃ��Ƃ�������ɗp����
	bool m_Invincible = false; //����Ȃǂ̖��G�����ǂ���

	int m_InvincibleFrame = 0;//���G���Ԃ���������t���[��

	DirectX::SimpleMath::Vector3 oldPosition;
	Colider* colme;     //�����̓����蔻��
	Colider* colattack;  //�U�����̓����蔻��
	DirectX::SimpleMath::Vector3 promissDirection{ 0.0f,0.0f,1.0f };//�v���C���[���U������ۂ̍ŏI�I�ɐU���������
	std::string m_Animname1="Idle"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(1)
	std::string m_Animname2="Walk"; //�A�j���[�V�����̍Đ����̃A�j���[�V�����w��𓮓I�ɍs�����߂̕ϐ�(2)

	class AttackObject* m_Child = {};

public:

	float alpha;//�v���C���[�̃A���t�@�l
	void Init() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;

	void Damage(float _damage)override;
	void STRecover();       //�X�^�~�i���񕜂���֐�
	void STUse(float);           //�X�^�~�i�������֐�
	void ASHP(float hp);//HP�𑝌�������֐�

	//�ȉ��͊e�ϐ����Ƃ̃Z�b�^�[&�Q�b�^�[
	 
	//���݂̃v���C���[�̏�Ԃ�Ԃ�
	PLAYERSTATE GetPstate() { return Pstate;}
	float GetST() { return ST; }
	bool GetWait() { return Wait; }
	Colider* Getcolattack() { return colattack; }
	float GetMaxST() { return MaxST; }
	float GetMaxHP() { return MaxHP; }
	float GetHP() { return HP; }
	bool GetInvincible() { return m_Invincible; }
	DirectX::SimpleMath::Vector3 GetpromissDirection() { return promissDirection; }
	DirectX::SimpleMath::Vector3 GetoldPosition() { return oldPosition; }

	void SetPstate(PLAYERSTATE s) { Pstate = s; }	//�v���C���[�̏�Ԃ�ς���B�����ɂ͕ς�������Ԃ�����
	void SetVerocity(float);
	void SetWait(bool wait) { Wait = wait; }
	void SetoldPosition(DirectX::SimpleMath::Vector3 pos) { oldPosition = pos; }
	void SetpromissDirection(DirectX::SimpleMath::Vector3 _dir) { promissDirection = _dir; }
	void SetInvincible(bool _invincible) { m_Invincible = _invincible; }//���G��Ԃɂ���(1f����,�J��Ԃ��Ďg���O��)
	void SetInvincibleFrame(int _frame) { m_InvincibleFrame = _frame; }//���G���Ԃ�ݒ肷��(�t���[��)
	void SetAnimName2(const char*);
	void SetAnimSpeed(float _speed)override { m_AnimSpeed = _speed; }//�A�j���[�V�����̍Đ����x�̐ݒ�
	void SetFrame1(int _frame) { m_Frame1 = _frame; }
	void SetFrame2(int _frame) { m_Frame2 = _frame; }
	std::unordered_map<const char*, Audio*> GetSEs() { return m_SE; }
	void PlaySE(const char* _SEname)override;
	void PlaySE(const char* _SEname,bool _loop)override;
	void StopSE(const char* _SEname)override;
	//DirectX::SimpleMath::Vector3 GetOldPos() { return oldPosition; }
};