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

//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];			// �{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			BoneWeight[4];
	int				BoneIdx[4];				// �{�[�����ւ̃C���f�b�N�X 20230909
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 AnimationMatrix1;
	aiMatrix4x4 AnimationMatrix2;
	aiMatrix4x4 OffsetMatrix;
	int			idx;						// 20230909
	aiQuaternion BlendRFrom;				// ���[�V�����u�����h����ۂ�FROM��9/25
	aiQuaternion BlendRTo;					// ���[�V�����u�����h����ۂ�TO��
	aiVector3D	BlendPosFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiVector3D	BlendPosTo;					// ���[�V�����u�����h����ۂ�TO��
};

// �{�[�� 20230909-02
struct CBBoneCombMatrx 
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// �{�[���R���r�l�[�V�����s��
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

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;			//�{�[���f�[�^�i���O�ŎQ�Ɓj
	std::unordered_map<std::string, GameObject*> m_BoneChild;//�{�[���̎q�I�u�W�F�N�g
	std::unordered_map<std::string, aiNode*> m_Nods; //�m�[�h�B

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

	ID3D11Buffer* m_BoneCombMtxCBuffer;						// �萔�o�b�t�@�@20230909-02
	void CreateIndexBufferPerMesh(int m, const aiMesh* mesh);		// �C���f�b�N�X�o�b�t�@�𐶐�
	void CreateVertexBufferPerMesh(int m, const aiMesh* mesh);		// ���_�o�b�t�@�𐶐�
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