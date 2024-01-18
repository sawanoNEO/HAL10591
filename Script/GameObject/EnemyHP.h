#pragma once
#include "gameObject.h"
class EnemyHP : public GameObject
{
private:

	float maxHP; //�ő�HP
	float m_damegePoint;
	float currentHP;//���݂�HP
	float pastHP;//HP�̌�������O�̒l
	int cnt;     //pastHP������͂��߂�܂ł̃J�E���g���Ƃ�
	bool damage; //��O�̃t���[���Ń_���[�W���󂯂Ă�����true
	struct HPGauge hpgauge;
	class Sprite* sprite;
public:

	void Init() override;
	void Update() override;
	void Draw() override;

	void deliverParamater(float _hp);
};

