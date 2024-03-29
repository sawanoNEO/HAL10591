#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"

#include "../GameObject/camera.h"
#include "../GameObject/player.h"
#include "../GameObject/enemy.h"
#include "../GameObject/box.h"

#include "../System/input.h"
#include "../ImGui/imguimanager.h"
#include <vector>

#include "../Component/Colider.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void Camera::Init()
{
	m_Position = Vector3(0.0f, 10.0f, -50.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
	angle = 270.0;
	Yangle = 60.0f;
	rock = false;
	//camcol = AddComponent<Colider>();//壁回避実装のためにカメラにも当たり判定を付ける
	//camcol->Init(Tug::DEFAULT,Vector3(0.1f,0.1f,0.1f));
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	Scene* nowscene = Manager::GetScene();
	Player* playerobj = nowscene->GetGameObject<Player>();
	Vector3 forward;
	Vector3 playerpos;
	Vector3 camforward = GetForward();

	std::vector<Enemy*> enemy = nowscene->GetGameObjects<Enemy>();//敵のオブジェクトを取得
	std::vector<Box*>box = nowscene->GetGameObjects<Box>();
	std::vector<Colider*>boxcol;
	boxcol.resize(box.size());
	for (int i = 0; i <boxcol.size() ; i++)
	{
		boxcol[i] = box[i]->GetComponent<Colider>();
	}
	/*camcol -> SetAABB(m_Position,0.1f,0.1f,0.1f);*/

	if (playerobj)//プレイヤーが消滅したときに誤って読み込まないようにしている
	{
		forward = playerobj->GetForward();
		playerpos = playerobj->GetPosition();
	}
	//球座標の公式を使用
	if (rock == false)
	{
		m_Position = Vector3(playerpos.x + (Distance * sin(Yangle * 3.14 / 180)) * cos(angle * 3.14 / 180), playerpos.y + (Distance * cos(Yangle * 3.14 / 180)), playerpos.z + (Distance * sin(Yangle * 3.14 / 180) * sin(angle * 3.14 / 180)));
		//m_GoalPosition = Vector3(playerpos.x + (Distance * sin(Yangle * 3.14 / 180)) * cos(angle * 3.14 / 180), playerpos.y + (Distance * cos(Yangle * 3.14 / 180)), playerpos.z + (Distance * sin(Yangle * 3.14 / 180) * sin(angle * 3.14 / 180)));
		//Vector3 goalvec = XMVector3Normalize(m_GoalPosition - m_Position);
		//
		//if (fabs((m_Position - m_GoalPosition).Length()) > 0.1)
		//{
		//	m_Position += goalvec * 0.5;
		//}
		//↓の5.0f*～というのは注視点がプレイヤーからどれだけ奥を見るかということ
		m_Target = Vector3(playerpos.x - (5.0f * sin(Yangle * 3.14 / 180)) * cos(angle * 3.14 / 180), playerpos.y - (5.0f * cos(Yangle * 3.14 / 180)), playerpos.z - (5.0f * sin(Yangle * 3.14 / 180) * sin(angle * 3.14 / 180)));
		//カメラの回転処理
		if (Input::GetStick(Input::RightX) > 0.1 || Input::GetStick(Input::RightX) < -0.1)
		{
			angle -= Input::GetStick(Input::RightX) * 3.0f;
		}
		if (Input::GetStick(Input::RightY) > 0.1 || Input::GetStick(Input::RightY) < -0.1)
		{
			Yangle += Input::GetStick(Input::RightY) * 1.5f;
			if (Yangle > 100.0f)
			{
				Yangle = 100.0f;
			}
			else if (Yangle < 45.0)
			{
				Yangle = 45.0f;
			}
		}
	}
	else// ターゲットカメラ
	{
		//m_Position = playerpos +- forward * 15;
		//m_Position.y += TargetY;
		//m_Target =RockEnemy->GetPosition();
		//if (abs((playerpos - m_Target).Length()) > 70)
		//{
		//	rock=false;
		//}
	}

	/*この式だとズームになる。謎
	m_Position = (Vector3(playerpos.x + (cos(angle * 3.14 / 180)*sin(Yangle*3.14/180)) * 15.0f, playerpos.y+sin(Yangle*3.14/180)*5.0, playerpos.z + (sin(angle * 3.14 / 180)*sin(Yangle*3.14/180)) * 15.0f));
	m_Target = (Vector3(playerpos.x - (cos(angle * 3.14 / 180)*sin(Yangle * 3.14 / 180)) * 15.0f, m_Position.y-sin(Yangle*3.14/180), playerpos.z - (sin(angle * 3.14 / 180)*sin(Yangle * 3.14 / 180)) * 15.0f));*/
	/*m_Position = (Vector3(playerpos.x + cos(angle * 3.14 / 180) * 15.0f, 5.0, playerpos.z + sin(angle * 3.14 / 180) * 15.0f));
	m_Target = (Vector3(playerpos.x - cos(angle * 3.14 / 180) * 15.0f, m_Position.y, playerpos.z - sin(angle * 3.14 / 180) * 15.0f));*/

	
	//右スティック押し込みで視点リセット
	if (Input::GetController(Input::RightStick, Input::PRESSED))
	{
		float kakudo = atan2(-forward.z, -forward.x);
		kakudo = kakudo * 180 / 3.14;
		angle = kakudo;
		Yangle =75.0f;
	}
	//if (Input::GetController(Input::RightStick, Input::PRESSED))////////////ロックオンカメラに切り替え
	//{
	//	RockEnemy = enemy[0];
	//	if (rock)
	//		rock = false;
	//	else
	//		rock = true;
	//}

	////壁回避の処理
	//Colider* wall;
	//
	//bool hit = false;
	//for (int i = 0; i < boxcol.size(); i++)
	//{
	//	Colider col = camcol->CollisionAABB(camcol->GetAABB(), boxcol[i]);
	//	if (col.GetTug() == Tug::WALL);
	//	{
	//		
	//	}
	//}

}

void Camera::Draw()
{
#if _DEBUG
	ImGui::Begin("Camera");
	ImGui::Text("Pos: x=%f,y=%f,z=%f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("Pos: x=%f,y=%f,z=%f", m_Scale.x, m_Scale.y, m_Scale.z);
	//ImGui::Text("Pos: x=%f,x=%f,y=%f,y=%f", camcol->GetAABB().max.x, camcol->GetAABB().min.x, camcol->GetAABB().max.z, camcol->GetAABB().min.z);
	ImGui::Text("Yangle=%f", Yangle);
	ImGui::SliderFloat("TargetY", &Distance, 5.0, 15.0);
	ImGui::End();
#endif

	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up);										// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角
	
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&projectionMatrix);

}

void Camera::Begin()
{
}

Vector3 Camera::GetCamForward()
{
	Vector3 Forward = XMVector3Normalize(m_Target - m_Position);
	return Forward;
}

Vector3 Camera::GetCamSide()
{
	Vector3 Side;
	Vector3 vec = GetCamForward();
	//x_rot = cos(θ) * x + sin(θ) * z
	//	y_rot = y
	//	z_rot = -sin(θ) * x + cos(θ) * z
	Side.x = cos(90 * 3.14 / 180) * vec.x + sin(90 * 3.14 / 180) * vec.z;
	Side.y = vec.y;
	Side.z = -sin(90 * 3.14 / 180) * vec.x + cos(90 * 3.14 / 180) * vec.z;

	return Side;
}

DirectX::SimpleMath::Vector3 Camera::VecYRemove(DirectX::SimpleMath::Vector3 vec)
{
	float XZ = fabsf(vec.x) + fabs(vec.z);
	float X_ratio = vec.x / XZ;
	float Z_ratio = vec.z / XZ;
	vec.x = X_ratio;
	vec.z = Z_ratio;

	vec.y = 0.0f;
	return XMVector3Normalize(vec);
	
}
