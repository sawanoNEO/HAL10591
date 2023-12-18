#pragma once

#include <unordered_map>
#include <vector>
#include <SimpleMath.h>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#pragma comment (lib, "assimp-vc142-mtd.lib")

#include "../Component/component.h"

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];			// 本来はボーンインデックスで管理するべき
	float			BoneWeight[4];
	int				BoneIdx[4];				// ボーン情報へのインデックス 20230909
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 AnimationMatrix1;
	aiMatrix4x4 AnimationMatrix2;
	aiMatrix4x4 OffsetMatrix;
	int			idx;						// 20230909
	aiQuaternion BlendRFrom;				// モーションブレンドする際のFROM側9/25
	aiQuaternion BlendRTo;					// モーションブレンドする際のTO側
	aiVector3D	BlendPosFrom;				// モーションブレンドする際のFROM側
	aiVector3D	BlendPosTo;					// モーションブレンドする際のTO側
};

// ボーン 20230909-02
struct CBBoneCombMatrx 
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// ボーンコンビネーション行列
};


class AnimationModel : public Component
{
private:
	static std::unordered_map<std::string, const aiScene*> loadedScenes;
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;
	DirectX::SimpleMath::Matrix ChildMatrix;

	ID3D11Buffer**	m_VertexBuffer;
	ID3D11Buffer**	m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）
	std::unordered_map<std::string, GameObject*> m_BoneChild;//ボーンの子オブジェクト
	std::unordered_map<std::string, aiNode*> m_Nods; //ノード達

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

	ID3D11Buffer* m_BoneCombMtxCBuffer;						// 定数バッファ　20230909-02
	void CreateIndexBufferPerMesh(int m, const aiMesh* mesh);		// インデックスバッファを生成
	void CreateVertexBufferPerMesh(int m, const aiMesh* mesh);		// 頂点バッファを生成
	aiMatrix4x4 GetCumulativeTransformation(const char* nodename);

public:
	using Component::Component;

	void Load( const char *FileName );
	void LoadAnimation( const char *FileName, const char *Name );
	bool CheckAnimData(const char* _Name);
	void Uninit() override;
	void Update(const char *AnimationName1, int Frame1, const char *AnimationName2, int Frame2, float BlendRate);
	void Draw() override;

	BONE* GetBONE(const char* _bonename);
	const aiScene* Getscene() { return m_AiScene; }
	template<typename T>
	T* AddBoneChild(const char* _bonename)
	{
		auto itr = m_Bone.find(_bonename);
		if (itr == m_Bone.end())
		{
			return nullptr;
		}
		T* child = new T;
		m_BoneChild.insert({ _bonename,child });
		((GameObject*)child)->Init();

		return child;
	}
	//void AddBoneChild(const char* _bonename, const char* objname);
	DirectX::SimpleMath::Matrix ChangeMatrix(aiMatrix4x4 _mat);
	DirectX::SimpleMath::Matrix GetBoneMatrix(const char* name);
};