#pragma once
#include "gameObject.h"
class ST : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	VERTEX_3D vertex[4];
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	MATERIAL material;////ƒ}ƒeƒŠƒAƒ‹

	int posX;
	int posY;

	class Shader* shader;
	class Sprite* sprite;

public:
	void Init();
	void Uninit();
	void Update();

};

