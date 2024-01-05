#include <SimpleMath.h>
#include "../System/main.h"
#include "../System/renderer.h"
#include "../System/utility.h"

#include "../ImGui/imguimanager.h"

#include "../Component/animationModel.h"

#include "../GameObject/gameObject.h"

#include <fstream>

using namespace DirectX::SimpleMath;
std::unordered_map<std::string, const aiScene*>AnimationModel::loadedScenes;

void AnimationModel::Draw()
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 20230909-02 レジスタ9にセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
			&m_Texture[path.data]);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
		Renderer::GetDeviceContext()->DrawIndexed(
			mesh->mNumFaces * 3, 0, 0);
	}

	for (auto& pair : m_BoneChild)
	{
		BONE bone = m_Bone[pair.first.c_str()];
		Matrix mat = ChangeMatrix(bone.Matrix);
		aiMatrix4x4 aaaa = bone.Matrix;
		for (auto itr = m_Nods[pair.first]; itr->mParent != nullptr; itr = itr->mParent)
		{
			aaaa *= m_Bone[itr->mParent->mName.C_Str()].Matrix;
			mat *= ChangeMatrix(m_Bone[itr->mParent->mName.C_Str()].Matrix);
		}
		Matrix result;
		result._11 = aaaa.a1; result._21 = aaaa.a2; result._31 = aaaa.a3; result._41 = aaaa.a4;
		result._12 = aaaa.b1; result._22 = aaaa.b2; result._32 = aaaa.b3; result._42 = aaaa.b4;
		result._13 = aaaa.c1; result._23 = aaaa.c2; result._33 = aaaa.c3; result._43 = aaaa.c4;
		result._14 = aaaa.d1; result._24 = aaaa.d2; result._34 = aaaa.d3; result._44 = aaaa.d4;
		result *= m_GameObject->GetMatrix();
		//mat._41 = bone.Matrix.a4;
		//mat._42 = bone.Matrix.b4;
		//mat._43 = bone.Matrix.c4;

		ImGui::Begin("model");
		ImGui::Text("Matrix11=%f,%f,%f,%f",mat._11, mat._21, mat._31, mat._41);
		ImGui::Text("Matrix21=%f,%f,%f,%f", mat._12, mat._22, mat._32, mat._42);
		ImGui::Text("Matrix31=%f,%f,%f,%f", mat._13, mat._23, mat._33, mat._43);
		ImGui::Text("Matrix41=%f,%f,%f,%f\n\n", mat._14, mat._24, mat._34, mat._44);
		ImGui::End();
		pair.second->DrawBase(result);

		int a = 0;
	}

	//一旦コメント
	//for (auto& pair : m_BoneChild)
	//{
	//	for (auto itr = m_Bone.find(pair.first.c_str()); itr != m_Bone.find("RootNode"); itr--)
	//	{
	//		BONE bone = itr->second;
	//		int a = 0;
	//	}
	//	
	//	Matrix mat = pair.second->GetMatrix();


	//	aiMatrix4x4 maat = m_Bone[m_BoneChild.begin()->first.c_str()].Matrix;
	//	mat = ChangeMatrix(maat);

	//	mat *= m_GameObject->GetMatrix();
	//	m_BoneChild.begin()->second->SetPosition(Vector3(maat.a1, maat.b1, maat.c1));
	//	//aiNode* node = m_Nods[pair.first.c_str()];
	//	//BONE bone = m_Bone[node->mName.C_Str()];
	//	//aiMatrix4x4 mat=bone.OffsetMatrix;
	//	//Matrix mat=-ChangeMatrix(node->mTransformation);

	//	//Matrix mat=ChangeMatrix(m_Bone[node->mName.C_Str()].Matrix);

	//	//mat *= m_GameObject->GetMatrix();

	//	//aiMatrix4x4 mat = m_Bone[pair.first.c_str()].AnimationMatrix;
	//	//Matrix result;
	//	//aiVector3D pos = aiVector3D(mat.d1,mat.d2,mat.d3);
	//    //ImGui::Text("Posx%fY%fZ%f", pos.z, pos.y, pos.z);
	//	//ImGui::Text("AnimationMatrix11=%f,%f,%f,%f", m_Bone[pair.first.c_str()].AnimationMatrix.a1, m_Bone[pair.first.c_str()].AnimationMatrix.a2, m_Bone[pair.first.c_str()].AnimationMatrix.a3, m_Bone[pair.first.c_str()].AnimationMatrix.a4);
	//	//ImGui::Text("AnimationMatrix21=%f,%f,%f,%f", m_Bone[pair.first.c_str()].AnimationMatrix.b1, m_Bone[pair.first.c_str()].AnimationMatrix.b2, m_Bone[pair.first.c_str()].AnimationMatrix.b3, m_Bone[pair.first.c_str()].AnimationMatrix.b4);
	//	//ImGui::Text("AnimationMatrix31=%f,%f,%f,%f", m_Bone[pair.first.c_str()].AnimationMatrix.c1, m_Bone[pair.first.c_str()].AnimationMatrix.c2, m_Bone[pair.first.c_str()].AnimationMatrix.c3, m_Bone[pair.first.c_str()].AnimationMatrix.c4);
	//	//ImGui::Text("AnimationMatrix41=%f,%f,%f,%f\n\n", m_Bone[pair.first.c_str()].AnimationMatrix.d1, m_Bone[pair.first.c_str()].AnimationMatrix.d2, m_Bone[pair.first.c_str()].AnimationMatrix.d3, m_Bone[pair.first.c_str()].AnimationMatrix.d4);
	// //   ImGui::Text("OffsetMatrix11=%f,%f,%f,%f", m_Bone[pair.first.c_str()].OffsetMatrix.a1, m_Bone[pair.first.c_str()].OffsetMatrix.a2, m_Bone[pair.first.c_str()].OffsetMatrix.a3, m_Bone[pair.first.c_str()].OffsetMatrix.a4);
	//	//ImGui::Text("OffsetMatrix21=%f,%f,%f,%f", m_Bone[pair.first.c_str()].OffsetMatrix.b1, m_Bone[pair.first.c_str()].OffsetMatrix.b2, m_Bone[pair.first.c_str()].OffsetMatrix.b3, m_Bone[pair.first.c_str()].OffsetMatrix.b4);
	//	//ImGui::Text("OffsetMatrix31=%f,%f,%f,%f", m_Bone[pair.first.c_str()].OffsetMatrix.c1, m_Bone[pair.first.c_str()].OffsetMatrix.c2, m_Bone[pair.first.c_str()].OffsetMatrix.c3, m_Bone[pair.first.c_str()].OffsetMatrix.c4);
	//	//ImGui::Text("OffsetMatrix41=%f,%f,%f,%f", m_Bone[pair.first.c_str()].OffsetMatrix.d1, m_Bone[pair.first.c_str()].OffsetMatrix.d2, m_Bone[pair.first.c_str()].OffsetMatrix.d3, m_Bone[pair.first.c_str()].OffsetMatrix.d4);
	//    ImGui::End();

	//	//result._11 = mat.a1; result._21 = mat.a2; result._31 = mat.a3; result._41 = mat.a4;
	//	//result._12 = mat.b1; result._22 = mat.b2; result._32 = mat.b3; result._42 = mat.b4;
	//	//result._13 = mat.c1; result._23 = mat.c2; result._33 = mat.c3; result._43 = mat.c4;
	//	//result._14 = mat.d1; result._24 = mat.d2; result._34 = mat.d3; result._44 = mat.d4;

	//	//result *= m_GameObject->GetMatrix();

	//	pair.second->DrawBase(mat);
	//}
	
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

DirectX::SimpleMath::Matrix AnimationModel::ChangeMatrix(aiMatrix4x4 _mat)
{
	Matrix result=Matrix::Identity;
	result._11 = _mat.a1; result._21 = _mat.a2; result._31 = _mat.a3; result._41 = _mat.a4;
	result._12 = _mat.b1; result._22 = _mat.b2; result._32 = _mat.b3; result._42 = _mat.b4;
	result._13 = _mat.c1; result._23 = _mat.c2; result._33 = _mat.c3; result._43 = _mat.c4;
	result._14 = _mat.d1; result._24 = _mat.d2; result._34 = _mat.d3; result._44 = _mat.d4;

	return result;
}

DirectX::SimpleMath::Matrix AnimationModel::GetBoneMatrix(const char* name)
{
	Matrix matrix;
	aiMatrix4x4 aimatrix = m_Bone.find(name)->second.Matrix;
	matrix._11 = aimatrix.a1;
	matrix._12 = aimatrix.a2;
	matrix._13 = aimatrix.a3;
	matrix._14 = aimatrix.a4;
	matrix._21 = aimatrix.b1;
	matrix._22 = aimatrix.b2;
	matrix._23 = aimatrix.b3;
	matrix._24 = aimatrix.b4;
	matrix._31 = aimatrix.c1;
	matrix._32 = aimatrix.c2;
	matrix._33 = aimatrix.c3;
	matrix._34 = aimatrix.c4;
	matrix._41 = aimatrix.d1;
	matrix._42 = aimatrix.d2;
	matrix._43 = aimatrix.d3;
	matrix._44 = aimatrix.d4;

	matrix = matrix.Transpose();

	return matrix;
}

// 頂点バッファ生成
void AnimationModel::CreateVertexBufferPerMesh(int m, const aiMesh* mesh) {

	VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

	//変形後頂点データ初期化
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

	//ボーンデータ生成
	for (unsigned int b = 0; b < mesh->mNumBones; b++)
	{
		aiBone* bone = mesh->mBones[b];

		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

		//変形後頂点にボーンデータ格納
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

	// 頂点データ生成
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

	// 頂点バッファ生成
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

aiMatrix4x4 AnimationModel::GetCumulativeTransformation(const char* nodename)
{
	aiMatrix4x4 cumulativeTransform;


	return aiMatrix4x4();
}

// インデックスバッファを生成
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

	std::ofstream outputFile("asset\\editer\\ModelData.csv");
	if (loadedScenes.find(FileName) != loadedScenes.end()) 
	{
		// 既にロードされている場合は保存された情報を返す
		m_AiScene = new aiScene(*loadedScenes[FileName]);
		//m_AiScene=loadedScenes[FileName];
	}
	else
	{
		loadedScenes[FileName] = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
		m_AiScene = new aiScene(*loadedScenes[FileName]);
		//m_AiScene = loadedScenes[FileName];
	}

	

	//if (outputFile.is_open()) 
	//{
	//	outputFile << FileName << std::endl;
	//	for (unsigned int i = 0; i < m_AiScene->mNumMeshes; ++i)
	//	{
	//	}
	//	outputFile.close();
	//}

	//m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	
	//outputFile.close();
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//再帰的にボーンの階層構造を生成（ボーン名で検索できる構造を作成）
	CreateBone(m_AiScene->mRootNode);

	// ボーンの配列位置を格納する				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		const aiMesh* mesh = m_AiScene->mMeshes[m];

		// インデックスバッファを生成
		CreateIndexBufferPerMesh(m, mesh);

		// 頂点バッファを生成
		CreateVertexBufferPerMesh(m, mesh);
	}

	//テクスチャ読み込み
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

	// 定数バッファ生成　20230909-02
	bool sts = CreateConstantBufferWrite(											// 20230909 - 02
		Renderer::GetDevice(),					    // デバイスオブジェクト			// 20230909 - 02
		sizeof(CBBoneCombMatrx),					// コンスタントバッファサイズ	// 20230909 - 02
		&m_BoneCombMtxCBuffer);						// コンスタントバッファ			// 20230909 - 02

	assert(m_BoneCombMtxCBuffer);

}


void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{
	if (loadedScenes.find(FileName) != loadedScenes.end())
	{
		// 既にロードされている場合は保存された情報を返す
		m_Animation[Name] = loadedScenes[FileName];
	}
	else
	{
		loadedScenes[FileName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
		m_Animation[Name] = loadedScenes[FileName];
	}

	//loadedScenes[FileName]= aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	//m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
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
	m_Nods[node->mName.C_Str()] = node;
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
	
	// アニメーションありか？
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];


 	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// 現在のアニメーション2について関連するボーンを全て更新
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//簡易実装
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// ブレンド
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		// 位置のブレンド
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//線形補間

		// 姿勢のブレンド
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//再帰的にボーンマトリクスを更新
//	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));


	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 ボーンコンビネーション行列の配列を生成する
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909



	//for (auto& pair : m_BoneChild)
	//{
	//	aiMatrix4x4 mat = m_Bone[pair.first.c_str()].Matrix;
	//	aiVector3D pos = aiVector3D(mat.d1, mat.d2, mat.d3);

	//	ChildMatrix._11 = mat.a1; ChildMatrix._21 = mat.a2; ChildMatrix._31 = mat.a3; ChildMatrix._41 = mat.a4;
	//	ChildMatrix._12 = mat.b1; ChildMatrix._22 = mat.b2; ChildMatrix._32 = mat.b3; ChildMatrix._42 = mat.b4;
	//	ChildMatrix._13 = mat.c1; ChildMatrix._23 = mat.c2; ChildMatrix._33 = mat.c3; ChildMatrix._43 = mat.c4;
	//	ChildMatrix._14 = mat.d1; ChildMatrix._24 = mat.d2; ChildMatrix._34 = mat.d3; ChildMatrix._44 = mat.d4;

	//	ChildMatrix *= m_GameObject->GetMatrix();

	//}



	// 20230909 転置
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// 転置する
		bcmtx.Transpose();
		int a = 0;
	}

	// 20230909-02 定数バッファに反映させる
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
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Bone[node->mName.C_Str()];
	
	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	//	worldMatrix *= matrix;						

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）


	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}



