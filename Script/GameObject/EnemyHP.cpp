#include "EnemyHP.h"

#include"camera.h"


#include "../Component/shader.h"
#include "../Component/sprite.h"

#include "../System/modelRenderer.h"

#include "../System/renderer.h"
#include "../System/main.h"
#include "../System/manager.h"

#include "../Scene/scene.h"

#include "../ImGui/imguimanager.h"

using namespace DirectX::SimpleMath;

ID3D11Buffer*EnemyHP::m_VertexBuffer;
ID3D11ShaderResourceView*EnemyHP::m_Texture;

void EnemyHP::Init()
{
	/*AddComponent<ModelRenderer>()->Load("asset\\model\\BillBoard.obj");*/

	VERTEX_3D vertex[4];

	vertex[0].Position = Vector3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(1.0f, -1.0f, 0.0f);
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
		L"asset/texture/grass.jpg",
		nullptr,
		&m_Texture);

	assert(m_Texture);

	AddComponent<Shader>()->Load("shader\\unlitTextureVS.cso", "shader\\PS_HPguage.cso");
	m_Position = Vector3{ 0.0f,10.0f,0.0f };
	m_Scale = Vector3{ 2.0f,0.001f,0.0f };
	maxHP = 100;
}



void EnemyHP::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	//HPGauge hpgauge{};
	Vector3 campos = camera->GetPosition();
	Vector3 forward = m_Position - campos;
	forward.Normalize();

	//m_Rotation.y = atan2(forward.x, forward.z);

	//親のHPデータを取得する
	if (!damage)//ダメージを受けていなかったら
	{
		pastHP = currentHP; //一つ前のフレームのHPを取得する
	}
	else if (damage && cnt < 30)
	{
		cnt++;
	}
	else if (cnt >= 30)
	{
		damage = false;
	}
	//m_damegePoint = player->GetHP();
	if (m_damegePoint > currentHP)
	{
		float gap = m_damegePoint - currentHP;
		currentHP += gap / 10;//HPのデータを更新する
	}
	else if (m_damegePoint < currentHP)
	{
		float gap = currentHP - m_damegePoint;
		currentHP -= gap / 10;//HPのデータを更新する
	}
	//if (pastHP > currentHP) //前のフレームのHPより現在のHPが下回っていたらダメージを受けたということになる
	//{
	//	damage = true;
	//}

	hpgauge.MaxHP = maxHP / 100;
	hpgauge.currentHP = currentHP / 100;
	hpgauge.pastHP = pastHP / 100;
	//Renderer::SetHPGauge(hpgauge);
}



void EnemyHP::Draw()
{
#if _DEBUG
	ImGui::Begin("EnemyHP");
	ImGui::Text("pos%f%f%f",m_Position.x,m_Position.y,m_Position.z);
	ImGui::Text("MaxHP%f",maxHP / 100);
	ImGui::Text("currentHP%f",currentHP / 100);
	ImGui::Text("currentHP%f",pastHP / 100);
	ImGui::SliderFloat("m_ScaleX", &m_Scale.x, 0.001, 2.0);
	ImGui::SliderFloat("m_ScaleY", &m_Scale.y, 0.001, 2.0);
	ImGui::SliderFloat("m_ScaleZ", &m_Scale.z, 0.001, 2.0);
	ImGui::End();
#endif
	Renderer::SetHPGauge(hpgauge);

	GetComponent<Shader>()->Draw();

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = Vector3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3(-1.0f,0.75f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3(1.0f, 0.75f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

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
}

void EnemyHP::deliverParamater(float _hp)
{
	m_damegePoint = _hp;
}
