#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/modelRenderer.h"
#include "../System/input.h"

#include "../GameObject/player.h"
#include "../GameObject/goal.h""
#include "../GameObject/camera.h"
#include "../GameObject/box.h"

#include "../Component/audio.h"
#include "../Component/shader.h"
#include "../Component/shadow.h"
#include "../Component/Colider.h"
#include "../Component/Rigidbody.h"
#include "../Component/animationModel.h"
#include "../Component/StateAttack.h"
#include "../Component/StateMove.h"
#include "../Component/StateNone.h"
#include "../Component/StateDash.h"
#include "../Component/StateRolling.h"
#include "../Component/StateItem.h"
#include "../Component/StateDamage.h"
#include "../Component/StateDeath.h"
#include "../Component/Heal.h"

#include "../ImGui/imguimanager.h"


using namespace DirectX::SimpleMath;

void Player::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");

	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset\\model\\Player\\Paladin J Nordstrom.fbx");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash.fbx", "Attack");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash2.fbx", "Attack2");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash3.fbx", "Attack3");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash4.fbx", "Attack4");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash5.fbx", "Attack5");
	//m_Model->LoadAnimation("asset\\model\\Player\\RightStrafe.fbx", "Simau");
	m_Model->LoadAnimation("asset\\model\\Player\\Stand To Roll.fbx", "Rolling");
	m_Model->LoadAnimation("asset\\model\\Player\\Impact.fbx", "Impact");
	m_Model->LoadAnimation("asset\\model\\Player\\Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield run (2).fbx", "Run2");
	m_Model->LoadAnimation("asset\\model\\Player\\Drinking.fbx", "Drink");
	m_Model->LoadAnimation("asset\\model\\Player\\Dancing.fbx", "Dance");
	m_Model->LoadAnimation("asset\\model\\Player\\RightStrafe.fbx", "RightStrafe");
	m_Model->LoadAnimation("asset\\model\\Player\\LeftStrafe.fbx", "LeftStrafe");
	m_Model->LoadAnimation("asset\\model\\Player\\Death.fbx", "Death");
	BONE* bone = m_Model->GetBONE("mixamorig:RightHand");
	

	//ステートマシンのaddcomponent
	AddComponent<StateNone>();
	AddComponent<StateMove>();//oldPositionの更新タイミングの関係上Coliderより先にaddcomponentする
	AddComponent<StateDash>();
	AddComponent<StateRolling>();
	AddComponent<StateAttack >();
	AddComponent<StateDamage >();
	AddComponent<StateDeath >();
	AddComponent<StateItem>();
	GetComponent<StateItem>()->Init(HEAL);
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<StateNone>());

	

	m_SE["Puntch1"] = AddComponent<Audio>();
	m_SE["Puntch1"]->Load("asset\\audio\\SE\\Puntch1.wav");
	m_SE["Puntch2"] = AddComponent<Audio>();
	m_SE["Puntch2"]->Load("asset\\audio\\SE\\Puntch2.wav");
	m_SE["Puntch3"] = AddComponent<Audio>();
	m_SE["Puntch3"]->Load("asset\\audio\\SE\\Puntch3.wav");
	m_SE["Swing1"] = AddComponent<Audio>();
	m_SE["Swing1"]->Load("asset\\audio\\SE\\Swing1.wav");
	m_SE["Swing2"] = AddComponent<Audio>();
	m_SE["Swing2"]->Load("asset\\audio\\SE\\Swing2.wav");
	m_SE["Swing3"] = AddComponent<Audio>();
	m_SE["Swing3"]->Load("asset\\audio\\SE\\Swing3.wav");
	m_SE["Walk"] = AddComponent<Audio>();
	m_SE["Walk"]->Load("asset\\audio\\SE\\砂の上1.wav");
	m_SE["Dash"] = AddComponent<Audio>();
	m_SE["Dash"]->Load("asset\\audio\\SE\\砂の上2.wav");
	m_SE["Heal"] = AddComponent<Audio>();
	m_SE["Heal"]->Load("asset\\audio\\SE\\治癒と蘇生の魔法・聖なる光.wav");
	m_SE["Rolling"] = AddComponent<Audio>();
	m_SE["Rolling"]->Load("asset\\audio\\SE\\回避.wav");
	m_SE["Miss"] = AddComponent<Audio>();
	m_SE["Miss"]->Load("asset\\audio\\SE\\miss.wav");

	m_Scale = Vector3(0.02f, 0.02f, 0.02f);
	m_Position = Vector3(0.0f, 0.0f, -10.0f);

	m_GroundHeight = 0.0f;
	//AddComponent<Move>();
	AddComponent<Rigidbody>();
	GetComponent<Rigidbody>()->Init(5.0);
	//AddComponent<Sword>();
	Vector3 max = GetComponent<ModelRenderer>()->GetMaxPos();
	Vector3 min = GetComponent<ModelRenderer>()->GetMinPos();
	Vector3 scale;
	scale.x = fabsf(max.x) + fabsf(min.x);
	scale.y = fabsf(max.y) + fabsf(min.y);
	scale.z = fabsf(max.z) + fabsf(min.z);
	m_Colme = AddComponent<Colider>();
	m_Colme->Init(PLAYER, Vector3(100.0f, 100.0f, 100.0f));

	m_HP = 1000;
	alpha = 1.0f;

}

void Player::Update()
{
	PlayerColor playercolor;
	playercolor.color.w = alpha;
	Renderer::SetPlayerColor(playercolor);
	// 前方ベクトルを取得
	Vector3 forward = GetForward();
	Vector3 fNormalR = GetSide();
	Vector3 fNormalL = -GetSide();

	const char* Animname1 = m_Animname1.c_str();//アニメーションの名前1
	const char* Animname2 = m_Animname2.c_str();//アニメーションの名前2

	//アニメーションの再生
	m_Model->Update(Animname1, m_Frame1, Animname2, m_Frame2, m_BlendRate);


	//回転処理
	if (m_PromissDirection.x * forward.x + m_PromissDirection.y * forward.y + m_PromissDirection.z * forward.z < 0.97 &&
		(fNormalR.x * m_PromissDirection.x + fNormalR.z * m_PromissDirection.z)>0) //右ベクトルとの内積
	{
		m_Rotation.y += 0.2;
	}
	else if (m_PromissDirection.x * forward.x + m_PromissDirection.y * forward.y + m_PromissDirection.z * forward.z < 0.97&&
		(fNormalL.x * m_PromissDirection.x + fNormalL.z * m_PromissDirection.z)>0)//左ベクトルとの内積
	{
		m_Rotation.y -= 0.2;
	}

	Vector3 vel = GetComponent<Rigidbody>()->GetVelocity();
	Vector3 acc = GetComponent<Rigidbody>()->GetAccel();
	Vector3 force = GetComponent<Rigidbody>()->GetForce();
	float mass = GetComponent<Rigidbody>()->GetMass();
	 //位置が０以下なら地面位置にセットする
	if (m_Position.y < m_GroundHeight && vel.y < 0.0f)
	{
		m_Position.y = m_GroundHeight;       //床の高さで止まる
		vel.y = 0.0f;                      //落ちる速度をリセット
		acc = force / mass;                //加速度を摩擦で減少させる
		GetComponent<Rigidbody>()->SetAccel(acc);
		GetComponent<Rigidbody>()->SetVelocity(vel);
	}

	//敵と接触していた場合に移動度を0にする処理
	std::array<std::list<Colider*>,HITDIRMAX> coliders = m_Colme->GetAllHitColiders();
	for (int i=0;i<HITDIRMAX;i++)
	{
		for (auto itr : coliders[i])
		{
			if (itr->GetTug() == ENEMY)
			{
				switch (i)
				{
				case TOP:
					m_Position.z = m_OldPosition.z;
					vel.z = 0.0;
					break;
				case BOTTOM:
					m_Position.z = m_OldPosition.z;
					vel.z = 0.0;
					break;
				case RIGHT:
					m_Position.x = m_OldPosition.x;
					vel.x = 0.0;
					break;
				case LEFT:
					m_Position.x = m_OldPosition.x;
					vel.x = 0.0;
					break;
				}
			}
		}
	}

	//アニメーションフレームを進める
	if (m_BlendRate < 1.0f)
	{
		m_BlendRate += 0.1f;
		m_Frame2+=1.0f * m_AnimSpeed;
	}
	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0;
	}
	if (m_BlendRate == 1.0f)
	{
		m_Frame1 += 1.0f * m_AnimSpeed;
	}

	//無敵時間が設定されていれば無敵にする
	if (m_InvincibleFrame > 0)
	{
		m_Invincible = true;
		m_InvincibleFrame--;
	}
	else
	{
		m_Invincible = false;
	}
}

void Player::Draw()
{
	//bool B = Input::GetController(Input::b, DirectX::GamePad::ButtonStateTracker::PRESSED);
	//bool up = Input::GetController(Input::LeftUP, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool down = Input::GetController(Input::LeftDown, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool left = Input::GetController(Input::LeftLeft, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool right = Input::GetController(Input::LeftRight, DirectX::GamePad::ButtonStateTracker::HELD);
#if _DEBUG

	ImGui::Begin("player");
	ImGui::SliderFloat("ST", &m_ST, 0, 100);
	ImGui::SliderFloat("HP", &m_HP, 0, 1000);
	ImGui::SliderFloat("Alpha", &alpha, 0, 1.0);
	ImGui::SliderFloat("posy", &m_Position.y, 0, 100.0);
	ImGui::SliderFloat("BlendRate", &m_BlendRate, 0, 1.0);
	ImGui::SliderFloat("m_Frame1", &m_Frame1, -10, 10);
	ImGui::SliderFloat("m_Frame2", &m_Frame2, -10, 10);
	ImGui::Text("Forward=x=%f,y=%f,z=%f", GetForward().x, GetForward().y, GetForward().z);
	ImGui::Text("Position=x=%f,y=%f,z=%f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("Blend%f", m_BlendRate);
	ImGui::Text("%c", m_Animname1);
	ImGui::Text("Matrix11%f,%f,%f,%f", GetMatrix()._11,GetMatrix()._12,GetMatrix()._13,GetMatrix()._14);
	ImGui::Text("Matrix21%f,%f,%f,%f", GetMatrix()._21,GetMatrix()._22,GetMatrix()._23,GetMatrix()._24);
	ImGui::Text("Matrix31%f,%f,%f,%f", GetMatrix()._31,GetMatrix()._32,GetMatrix()._33,GetMatrix()._34);
	ImGui::Text("Matrix41%f,%f,%f,%f", GetMatrix()._41,GetMatrix()._42,GetMatrix()._43,GetMatrix()._44);
	//ImGui::Text("%f\n", m_Position.x);
	//ImGui::Text("%f\n", m_Position.y);
	//ImGui::Text("%f\n", m_Position.z);
	//ImGui::Text("OldPos\n", m_Position.z);
	//ImGui::Text("x%f\n", oldPosition.x);
	//ImGui::Text("y%f\n", oldPosition.y);
	//ImGui::Text("z%f\n", oldPosition.z);
	//ImGui::Text("RotationY=%f\n", m_Rotation.y);
	//ImGui::Text("ST%f\n", ST);
	//ImGui::Text("max.x%f\n", c->GetAABB().max.x);
	//ImGui::Text("min.x%f\n", c->GetAABB().min.x);
	//ImGui::Text("max.z%f\n", c->GetAABB().max.z);
	//ImGui::Text("min.z%f\n", c->GetAABB().min.z);	
	//ImGui::Text("\n\nmax.x%f\n", co->GetAABB().max.x);
	//ImGui::Text("min.x%f\n", co->GetAABB().min.x);
	//ImGui::Text("max.z%f\n", co->GetAABB().max.z);
	//ImGui::Text("min.z%f\n", co->GetAABB().min.z);
	//ImGui::Text("HP=%i\n",HP);
	//ImGui::Text("min.z%f\n", co->GetAABB().min.z);
	//ImGui::Checkbox("PadB", &B);
	//ImGui::Checkbox("Padup", &up);
	//ImGui::Checkbox("Paddown", &down);
	//ImGui::Checkbox("Padleft", &left);
	//ImGui::Checkbox("Padright", &right);
	ImGui::End();
#endif
}

bool Player::Damage(float _damage)
{
	if (m_Invincible == false && m_HP > 0)//既に死んでると通らない
	{
		m_HP -= _damage;
		if (m_HP <= 0)
		{
			GetComponent<StateMachine>()->changeState(GetComponent<StateDeath>());
		}
		else
		{
			GetComponent<StateMachine>()->changeState(GetComponent<StateDamage>());
		}
		return true;
	}
	return false;
}

void Player::STRecover()
{
	m_ST += m_RecoverST;
	if (m_ST >= m_MaxST)
	{
		m_ST = m_MaxST;
		m_Wait = false;
	}
}

void Player::STUse(float c)
{
	m_ST -= c;
	if (m_ST < 0)
	{
		m_ST = 0;
		m_Wait = true;
	}
}

void Player::SetVerocity(float v)
{
	m_Velocity.y = v;
}


void Player::ASHP(float hp)
{
	m_HP += hp;
	if (m_HP > m_MaxHP)
	{
		m_HP = m_MaxHP;
	}
}

void Player::SetInvincible(bool _invincible)
{
	m_Invincible = _invincible; 
	if (m_Invincible == true)
	{
		m_InvincibleFrame = 1;
	}
}

void Player::SetAnimName2(const char* _Name)
{
	assert(m_Model->CheckAnimData(_Name)!=false&&"指定のアニメーションが見つかりませんでした。引数の名前や、データが入っているか確認してください。");

	if (m_Animname2 == _Name)
	{
		return;
	}

	//再生アニメーションの変更、各アニメーション関連の変数のリセット
	m_Animname1 = m_Animname2;
	m_Animname2 = _Name;
	m_Frame1 = m_Frame2;
	m_Frame2 = 0;
	m_BlendRate = 0.0;
}

void Player::PlaySE(const char* _SEname)
{
	m_SE[_SEname]->Play();
}

void Player::PlaySE(const char* _SEname, bool _loop)
{
	m_SE[_SEname]->Play(_loop);
}

void Player::StopSE(const char* _SEname)
{
	m_SE[_SEname]->Stop();
}
