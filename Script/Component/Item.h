#pragma once
#include "component.h"

enum ITEMID//�A�C�e���̎��
{
	HEAL,
};

class Item
{
protected:
	int m_Count = 0;//�A�C�e���g�p���삪��f�����Ă��邩
	int m_InventoryCount;//�A�C�e��������
	int m_MaxInventoryCount;//�A�C�e���ő及����
	int m_StartUpTime;//���ʂ����������܂ł̃t���[��
	int m_ActionTime;//���ʂ���������Ă���t���[��
	int m_RecoveryTime;//�A�C�e���g�p��̍d��
	bool m_ChangeStateFlg=false;//��Ԃ̐؂�ւ��𔻒f
	const char* m_AnimName;//�e�A�C�e���ɘA�����Ă���A�j���[�V�����̖��O
	class GameObject* m_GameObject;

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
	virtual void Draw() = 0;
	void SetGameObject(GameObject* _gameobject) { m_GameObject = _gameobject; }

	const char* GetAnimName() { return m_AnimName; }
	bool GetChangeFlg() { return m_ChangeStateFlg; }
};

