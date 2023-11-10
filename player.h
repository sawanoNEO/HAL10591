#pragma once
#include "gameObject.h"

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
	class Audio*	m_SE{};

	class AnimationModel* m_Model;
	int	m_Frame;
	float	m_BlendRate;

	float MaxHP=1000;     //�ő�̗�
	float HP;             //���ݑ̗�
	float groundHeight; //���̍���
	float ST;           //�X�^�~�i
	float MaxST = 100;        //�X�^�~�i�ő�l
	float Speed;        //��{�̈ړ����x
	float Accel;        //�ʏ�̉����x
	float DAccel;       //�_�b�V�����̉����x
	float MaxSpeed;     //�ő�ړ����x
	float cnt;          //�e����̃J�E���g
	bool Action=false;  //�X�^�~�i�������s�������Ă��邩
	bool Wait = false;  //�_�b�V���ŃX�^�~�i���g���؂�������true�ɂȂ�
	DirectX::SimpleMath::Vector3 oldPosition;
	Colider* colme;     //�����̓����蔻��
	Colider* colattack;  //�U�����̓����蔻��
	DirectX::SimpleMath::Vector3 promissDirection{ 0.0f,0.0f,1.0f };//�v���C���[���U������ۂ̍ŏI�I�ɐU���������

public:

	float alpha;//�v���C���[�̃A���t�@�l
	void Init() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;

	void STRecover(float);       //�X�^�~�i���񕜂���֐�
	void STUse(float);           //�X�^�~�i�������֐�

	//�ȉ��͊e�ϐ����Ƃ̃Z�b�^�[&�Q�b�^�[
	 
	//���݂̃v���C���[�̏�Ԃ�Ԃ�
	PLAYERSTATE GetPstate() { return Pstate;}
	//�v���C���[�̏�Ԃ�ς���B�����ɂ͕ς�������Ԃ�����
	void SetPstate(PLAYERSTATE s) { Pstate = s; }
	float GetST() { return ST; }
	void SetVerocity(float);
	bool GetWait() { return Wait; }
	void SetWait(bool wait) { Wait = wait; }
	Colider* Getcolattack() { return colattack; }
	float GetMaxST() { return MaxST; }

	float GetMaxHP() { return MaxHP; }
	float GetHP() { return HP; }
	void ASHP(float hp);

	DirectX::SimpleMath::Vector3 GetpromissDirection() { return promissDirection; }
	void SetpromissDirection(DirectX::SimpleMath::Vector3 _dir) { promissDirection = _dir; }

	//DirectX::SimpleMath::Vector3 GetOldPos() { return oldPosition; }
};