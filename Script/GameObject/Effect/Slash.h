#pragma once
#include "../gameObject.h"
#include <unordered_map>



enum SlashType
{
	White,
	Red
};

class Slash : public GameObject
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	static std::unordered_map<const char*,ID3D11ShaderResourceView*> m_Textures;

	int m_Count;
public:
	static void Load();
	static void Unload();

	void Init() override;
	void Update() override;
	void Draw() override;
	void SetColor(SlashType);

};
