#pragma once
#include "../gameObject.h"

class Puntch : public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	int m_Count;
	float m_PlaySpeed;
public:
	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;
	void Draw() override;

};

