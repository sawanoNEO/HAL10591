#pragma once
#include "gameObject.h"
class EnemyHP : public GameObject
{
private:

	float m_MaxHP; //最大HP
	float m_damegePoint;
	float currentHP;//現在のHP
	float pastHP;//HPの減少する前の値
	int cnt;     //pastHPが減りはじめるまでのカウントをとる
	bool damage; //一つ前のフレームでダメージを受けていたらtrue
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

