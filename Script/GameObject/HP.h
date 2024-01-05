#pragma once
#include "gameObject.h"
class HP : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	VERTEX_3D vertex[4];
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	MATERIAL material;////マテリアル

	int posX; //横軸の位置
	int posY; //縦軸の位置

	float maxHP; //最大HP
	float m_damegePoint;
	float currentHP;//現在のHP
	float pastHP;//HPの減少する前の値
	int cnt;     //pastHPが減りはじめるまでのカウントをとる
	bool damage; //一つ前のフレームでダメージを受けていたらtrue

	class Shader* shader;
	class Sprite* sprite;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

