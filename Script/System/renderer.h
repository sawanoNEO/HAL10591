#pragma once

#include	<d3d11.h>
#include	<SimpleMath.h>
#include	"../System/Application.h"
#include    <list>
// �����N���ׂ��O�����C�u����
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// �R�c���_�f�[�^
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int			BoneIndex[4];						// 20230909-02
	float		BoneWeight[4];						// 20230909-02
};


// �}�e���A��
struct MATERIAL
{
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Specular;
	DirectX::SimpleMath::Color	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2]{};
};

// ���s����
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::SimpleMath::Vector4	Direction;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Ambient;
};

struct Polar
{
	float gauge1; //�Q�[�W�̉����l
	float gauge2; //�Q�[�W�̏���l
	float inner; //�`�悷������̉~�̔��a
	float outer; //�`�悷��O���̉~�̔��a
	DirectX::SimpleMath::Vector4 baseColor; //�x�[�X�J���[
	DirectX::SimpleMath::Vector4 diffColor; //�Q�[�W��gauge1��gauge2�̊Ԃ̎��̐F
	DirectX::SimpleMath::Vector4 lostColor; //�Q�[�W��gauge2�𒴂������̐F

};

struct Gauge
{
	float currentST;  //���݂̃X�^�~�i
	float MaxST;      //�ő�X�^�~�i
	DirectX::SimpleMath::Vector2 dummy;
	DirectX::SimpleMath::Vector4 baseColor;  //�x�[�X�J���[(��)
	DirectX::SimpleMath::Vector4 lostColor;  //�X�^�~�i���Ȃ��Ȃ����Ƃ��̐F
};

struct HPGauge
{
	float currentHP;  //���݂�HP
	float MaxHP;      //�ő�HP
	float pastHP;     //�Ȃ��Ȃ�O��HP
	float dummy;      //�_�~�[
	DirectX::SimpleMath::Vector4 baseColor;  //�x�[�X�J���[(��)
	DirectX::SimpleMath::Vector4 lostColor;  //HP���Ȃ��Ȃ����Ƃ��̐F
	DirectX::SimpleMath::Vector4 gupColor;  //��������HP�̐F
};

struct PlayerColor
{
	DirectX::SimpleMath::Vector4 color;
};

// �����_��
class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_PolarBuffer;
	static ID3D11Buffer*			m_GaugeBuffer;
	static ID3D11Buffer*			m_HPGaugeBuffer;
	static std::list<ID3D11Buffer*>	m_HPGaugeBuffer2;
	static ID3D11Buffer*			m_PlayerColorBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;

	static Application* m_Application;

public:
	static void Init(Application* ap);
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
	static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetPolar(Polar polar);
	static void SetGauge(Gauge gauge);  //�X�^�~�i�Q�[�W
	static void SetHPGauge(HPGauge hpgauge);  //HP�Q�[�W
	static void SetPlayerColor(PlayerColor playercolor);  //�v���C���[�J���[
	static void AddHPGuage(ID3D11Buffer* _hpbuffer) { m_HPGaugeBuffer2.push_back(_hpbuffer); }

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};
