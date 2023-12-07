#include <SimpleMath.h>
#include "../System/main.h"
#include "../System/renderer.h"
#include "../Component/animationModel.h"
#include "../System/utility.h"

using namespace DirectX::SimpleMath;

void AnimationModel::Draw()
{
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 20230909-02 ���W�X�^9�ɃZ�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// �e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
			&m_Texture[path.data]);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(
			mesh->mNumFaces * 3, 0, 0);
	}
}

BONE* AnimationModel::GetBONE(const char* _bonename)
{
	auto itr = m_Bone.find(_bonename);
	if (itr != m_Bone.end()) 
	{
		BONE bone = itr->second;
		return &bone;
	}
	return nullptr;
}

// ���_�o�b�t�@����
void AnimationModel::CreateVertexBufferPerMesh(int m, const aiMesh* mesh) {

	VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

	//�ό`�㒸�_�f�[�^������
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		DEFORM_VERTEX deformVertex;
		deformVertex.Position = mesh->mVertices[v];
		deformVertex.Normal = mesh->mNormals[v];
		deformVertex.BoneNum = 0;

		for (unsigned int b = 0; b < 4; b++)
		{
			deformVertex.BoneName[b] = "";
			deformVertex.BoneWeight[b] = 0.0f;
		}

		m_DeformVertex[m].push_back(deformVertex);
	}

	//�{�[���f�[�^����
	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		aiBone* bone = mesh->mBones[b];

		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

		//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
		for (unsigned int w = 0; w < bone->mNumWeights; w++)
		{
			aiVertexWeight weight = bone->mWeights[w];

			int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

			m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
			m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();

			// 20230909
			m_DeformVertex[m][weight.mVertexId].BoneIdx[num] = m_Bone[bone->mName.C_Str()].idx;

			m_DeformVertex[m][weight.mVertexId].BoneNum++;

			assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
		}
	}

	// ���_�f�[�^����
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		vertex[v].Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
		vertex[v].Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
		vertex[v].TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
		vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

		vertex[v].BoneIndex[0] = m_DeformVertex[m][v].BoneIdx[0];
		vertex[v].BoneIndex[1] = m_DeformVertex[m][v].BoneIdx[1];
		vertex[v].BoneIndex[2] = m_DeformVertex[m][v].BoneIdx[2];
		vertex[v].BoneIndex[3] = m_DeformVertex[m][v].BoneIdx[3];

		vertex[v].BoneWeight[0] = m_DeformVertex[m][v].BoneWeight[0];
		vertex[v].BoneWeight[1] = m_DeformVertex[m][v].BoneWeight[1];
		vertex[v].BoneWeight[2] = m_DeformVertex[m][v].BoneWeight[2];
		vertex[v].BoneWeight[3] = m_DeformVertex[m][v].BoneWeight[3];
	}

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(
		&bd,
		&sd,
		&m_VertexBuffer[m]);

	delete[] vertex;
}

// �C���f�b�N�X�o�b�t�@�𐶐�
void AnimationModel::CreateIndexBufferPerMesh(int m, const aiMesh* mesh) {

	unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

	for (unsigned int f = 0; f < mesh->mNumFaces; f++)
	{
		const aiFace* face = &mesh->mFaces[f];

		assert(face->mNumIndices == 3);

		index[f * 3 + 0] = face->mIndices[0];
		index[f * 3 + 1] = face->mIndices[1];
		index[f * 3 + 2] = face->mIndices[2];
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

	delete[] index;

}

void AnimationModel::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//�ċA�I�Ƀ{�[���̊K�w�\���𐶐��i�{�[�����Ō����ł���\�����쐬�j
	CreateBone(m_AiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[����				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		const aiMesh* mesh = m_AiScene->mMeshes[m];

		// �C���f�b�N�X�o�b�t�@�𐶐�
		CreateIndexBufferPerMesh(m, mesh);

		// ���_�o�b�t�@�𐶐�
		CreateVertexBufferPerMesh(m, mesh);
	}

	//�e�N�X�`���ǂݍ���
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		ID3D11ShaderResourceView* texture;

		aiTexture* aitexture = m_AiScene->mTextures[i];

		// mHeight is 0, so try to load a compressed texture of mWidth bytes
		const size_t size = aitexture->mWidth;

		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			Renderer::GetDevice(),
			Renderer::GetDeviceContext(),
			reinterpret_cast<const unsigned char*>(aitexture->pcData),
			aitexture->mWidth,
			nullptr,
			&texture);

		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	// �萔�o�b�t�@�����@20230909-02
	bool sts = CreateConstantBufferWrite(											// 20230909 - 02
		Renderer::GetDevice(),					    // �f�o�C�X�I�u�W�F�N�g			// 20230909 - 02
		sizeof(CBBoneCombMatrx),					// �R���X�^���g�o�b�t�@�T�C�Y	// 20230909 - 02
		&m_BoneCombMtxCBuffer);						// �R���X�^���g�o�b�t�@			// 20230909 - 02

	assert(m_BoneCombMtxCBuffer);

}


void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{

	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);

}

bool AnimationModel::CheckAnimData(const char* _Name)
{
	if (m_Animation.count(_Name))
	{
		return true;
	}
	return false;
}


void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}


void AnimationModel::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}

	m_BoneCombMtxCBuffer->Release();						// 20230909-02

	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}
}

void AnimationModel::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
{

	// �A�j���[�V�������肩�H
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];


	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// ���݂̃A�j���[�V����2�ɂ��Ċ֘A����{�[����S�čX�V
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// �u�����h
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		// �ʒu�̃u�����h
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//���`���

		// �p���̃u�����h
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//���ʐ��`���

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
//	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 �{�[���R���r�l�[�V�����s��̔z��𐶐�����
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909

	// 20230909 �]�u
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// �]�u����
		bcmtx.Transpose();
	}

	// 20230909-02 �萔�o�b�t�@�ɔ��f������
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}

}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��

	//	worldMatrix *= matrix;						

	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j


	bone->Matrix = worldMatrix;					// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

