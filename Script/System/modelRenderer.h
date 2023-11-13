#pragma once
#include <string>
#include <unordered_map>
#include "../Component/component.h"
#include "utftosjisconv.h"
#include "../System/renderer.h"
#include <SimpleMath.h>

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};

class ModelRenderer : public Component
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	static DirectX::SimpleMath::Vector3 MaxPos; //���_���W�̍ő�̏ꏊ
	static DirectX::SimpleMath::Vector3 MinPos; //���_���W�̍ő�̏ꏊ
	
	MODEL* m_Model{};

public:

	static void Preload( const char *FileName );
	static void UnloadAll();

	DirectX::SimpleMath::Vector3 GetMaxPos() { return MaxPos; }
	DirectX::SimpleMath::Vector3 GetMinPos() { return MinPos; }

	using Component::Component;

	void Load( const char *FileName );

	void Draw() override;
};