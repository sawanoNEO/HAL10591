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
	MATERIAL material;////�}�e���A��

	int posX; //�����̈ʒu
	int posY; //�c���̈ʒu

	float maxHP; //�ő�HP
	float m_damegePoint;
	float currentHP;//���݂�HP
	float pastHP;//HP�̌�������O�̒l
	int cnt;     //pastHP������͂��߂�܂ł̃J�E���g���Ƃ�
	bool damage; //��O�̃t���[���Ń_���[�W���󂯂Ă�����true

	class Shader* shader;
	class Sprite* sprite;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

