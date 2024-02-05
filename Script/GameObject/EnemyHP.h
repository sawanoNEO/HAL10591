#pragma once
#include "gameObject.h"
class EnemyHP : public GameObject
{
private:

	float m_MaxHP; //�ő�HP
	float m_damegePoint;
	float currentHP;//���݂�HP
	float pastHP;//HP�̌�������O�̒l
	int cnt;     //pastHP������͂��߂�܂ł̃J�E���g���Ƃ�
	bool damage; //��O�̃t���[���Ń_���[�W���󂯂Ă�����true
	struct HPGauge hpgauge;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

public:

	void Init() override;
	void Update() override;
	void Draw() override;

	void deliverParamater(float _hp);
	void deliverMaxHP(float _maxhp) { m_MaxHP = _maxhp; }
};

