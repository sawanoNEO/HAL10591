#include "Slash.h"

#include"../camera.h"

#include "../../Component/shader.h"
#include "../../Component/sprite.h"

#include "../../System/renderer.h"
#include "../../System/main.h"
#include "../../System/manager.h"

#include "../../Scene/scene.h"

#include "../../ImGui/imguimanager.h"

ID3D11Buffer* Slash::m_VertexBuffer{};
ID3D11ShaderResourceView* Slash::m_Texture{};
std::unordered_map<const char*, ID3D11ShaderResourceView*> Slash::m_Textures;

using namespace DirectX::SimpleMath;

void Slash::Load()
{


	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(-2.0f, 2.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(2.0f, 2.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(-2.0f, -2.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(2.0f, -2.0f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/PuntchN.png",
		nullptr,
		&m_Texture);
	m_Textures["SlashW"] = m_Texture;

	assert(m_Texture);

	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		L"asset/texture/PuntchR.png",
		nullptr,
		&m_Texture);
	m_Textures["SlashR"] = m_Texture;

	assert(m_Texture);


}



void Slash::Unload()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

}



void Slash::Init()
{
	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

}



void Slash::Update()
{
	m_Count++;

	if (m_Count >= 10)
	{
		SetDestroy();
		return;
	}

}



void Slash::Draw()
{
#if _DEBUG
	ImGui::Begin("Slash");
	ImGui::Text("aaaaa");
	ImGui::End();
#endif

	Renderer::SetBlendState(BS_ADD);

	//テクスチャ座標算出
	float x = m_Count % 10 * (1.0f / 10);

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = Vector3(-2.0f, 2.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(x, 0.0f);

	vertex[1].Position = Vector3(2.0f, 2.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(x + (1.0/10), 0.0f);

	vertex[2].Position = Vector3(-2.0f, -2.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(x, 1.0f);

	vertex[3].Position = Vector3(2.0f, -2.0f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(x + (1.0 / 10),1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	Matrix view = camera->GetViewMatrix();

	// ビューの逆行列
	Matrix invView;
	invView = view.Invert();		//逆行列

	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// ワールドマトリクス設定
	Matrix world, scale, /*rot,*/ trans;

	scale = Matrix::CreateScale(m_Scale.x);
	trans = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	Renderer::SetBlendState(BS_ALPHA);
}

void Slash::SetColor(SlashType _type)
{
	switch (_type)
	{
	case White:
		m_Texture = m_Textures["SlashW"];
		break;
	case Red:
		m_Texture = m_Textures["SlashR"];
		break;
	default:
		break;
	}
}
