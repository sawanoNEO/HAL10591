#pragma once
#include "component.h"
#include "Colider.h"
class Sword : public Component
{
private:
	int Maxcombo;       //�A���ōU�������Ƃ��̃��[�V�����̐�
	int combo;          //���݂̍ő�R���{
	int Startup;        //�U���̑O���̃t���[����
	int ActiveFrames;   //�U���̎����t���[����
	int Recovery;       //�U���̌㌄�̃t���[����
	int cnt;            //���݂ǂꂾ���U���̃��[�V�������i��ł��邩
	int Power;          //�U����
	float STconsumption;//�����X�^�~�i�̗�

	AABB2 ab;
public:
	using Component::Component;
	void Init();
	void Update()override;
	void Draw()override;

};

