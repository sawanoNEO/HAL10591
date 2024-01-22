#include "../System/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/modelRenderer.h"
#include "../System/input.h"

#include "../GameObject/player.h"
#include "../GameObject/goal.h""
#include "../GameObject/camera.h"
#include "../GameObject/cylinder.h"
#include "../GameObject/box.h"
#include "../GameObject/bullet.h"
#include "../GameObject/AttackObject.h"

#include "../Component/audio.h"
#include "../Component/shader.h"
#include "../Component/shadow.h"
#include "../Component/Jump.h"
#include "../Component/Move.h"
#include "../Component/Sword.h"
#include "../Component/Colider.h"
#include "../Component/collision.h"
#include "../Component/Rigidbody.h"
//#include "../Component/Est.h"
#include "../Component/Rolling.h"
#include "../Component/animationModel.h"
#include "../Component/ModelManager.h"
#include "../Component/StateMove.h"
#include "../Component/StateNone.h"
#include "../Component/StateDash.h"
#include "../Component/StateRolling.h"
#include "../Component/StateAttack.h"
#include "../Component/StateItem.h"
#include "../Component/Heal.h"

#include "../ImGui/imguimanager.h"


using namespace DirectX::SimpleMath;

void Player::Init()
{
	AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\vertexLightingPS.cso");
	//AddComponent<Shader>()->Load("shader\\vertexLightingOneSkinVS.cso", "shader\\PS_Player.cso");

	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset\\model\\Player\\Paladin J Nordstrom.fbx");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Player\\Sword And Shield Walk.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\Player\\Slash.fbx", "Attack");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield slash.fbx", "Attack2");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield slash (3).fbx", "Attack3");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield slash (4).fbx", "Attack4");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield slash (5).fbx", "Attack5");
	m_Model->LoadAnimation("asset\\model\\Player\\kensimau.fbx", "Simau");
	m_Model->LoadAnimation("asset\\model\\Player\\Stand To Roll.fbx", "Rolling");
	m_Model->LoadAnimation("asset\\model\\Player\\Impact.fbx", "Impact");
	m_Model->LoadAnimation("asset\\model\\Player\\Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Player\\sword and shield run (2).fbx", "Run2");
	m_Model->LoadAnimation("asset\\model\\Player\\Drinking.fbx", "Drink");
	BONE* bone = m_Model->GetBONE("mixamorig:RightHand");
	
	// シーンからボーンのルートノードを取得
	//aiNode* boneRootNode = m_Model->Getscene()->mRootNode->FindNode("mixamorig:RightHand");
	//m_Model->AddBoneChild<AttackObject>("RootNode");
	//m_Model->AddBoneChild<AttackObject>("mixamorig:RightHand");
	//m_Model->AddBoneChild<AttackObject>("mixamorig:Spine");

	//ステートマシンのテスト
	AddComponent<StateNone>();
	AddComponent<StateMove>();//oldPositionの更新タイミングの関係上Coliderより先にaddcomponentする
	AddComponent<StateDash>();
	AddComponent<StateRolling>();
	AddComponent<StateAttack >();
	AddComponent<StateItem>();
	GetComponent<StateItem>()->Init(HEAL);
	AddComponent<StateMachine>();
	GetComponent<StateMachine>()->Init(GetComponent<StateNone>());

	//AddComponent<ModelManager>();
	{
		//m_Model->Load("asset\\model\\Standing Taunt Battlecry.fbx");				// animation ok
		////m_Model->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Idle");
		////m_Model->LoadAnimation("asset\\model\\Standing Taunt Battlecry.fbx", "Run");
		//m_Model->LoadAnimation("asset\\model\\Player\\sword and shield idle.fbx","Idle");
		//m_Model->LoadAnimation("asset\\model\\Player\\sword and shield kick.fbx","Run");



		//	m_Model->Load("asset\\model\\Akai2.fbx");									// animation ok
		//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Idle");
		//	m_Model->LoadAnimation("asset\\model\\Akai_Walk.fbx", "Run");

		//	m_Model->Load("data\\model\\Walking\\Walking2.fbx");						// animation ok
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Idle");
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking2.fbx", "Run");

		//	m_Model->Load("data\\model\\Walking\\Walking.fbx");							// animation ok
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Idle");
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.fbx", "Run");

		//	m_Model->Load("data\\model\\Walking\\Walking.dae");							// animation ok
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Idle");		// animation ok
		//	m_Model->LoadAnimation("data\\model\\Walking\\Walking.dae", "Run");			// animation ok

	}
	//AddComponent<ModelRenderer>()->Load("asset\\model\\player.obj");

	//AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset\\audio\\wan.wav");

	m_Scale = Vector3(0.02f, 0.02f, 0.02f);
	m_Position = Vector3(0.0f, 0.0f, -10.0f);

	groundHeight = 0.0f;
	AddComponent<Jump>();
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
	colme = AddComponent<Colider>();
	colme->Init(PLAYER, Vector3(100.0f, 100.0f, 100.0f));

	colattack=AddComponent<Colider>();
	colattack->Init(PLAYER, Vector3(1.0f,1.0f,1.0f));
	//AddComponent<Est>();
	HP = 1000;
	//AddComponent<Rolling>();
	alpha = 1.0f;

	//m_Child = AddChild<AttackObject>();
	//m_Child = AddChild<GameObject>();
	//m_Child->AddComponent<Shader>()->Load("shader\\vertexLightingVS.cso", "shader\\unlitTexturePS.cso");
	//m_Child->AddComponent<ModelRenderer>()->Load("asset\\model\\Player\\Sword.obj");
	//m_Child->SetPosition(Vector3{ 30.0f, 40.0f, 0.0f });
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

	if (Input::GetKeyTrigger('M'))
	{
		SetAnimName2("Simau");
	}
	if (Input::GetKeyTrigger('1'))
	{
		SetAnimName2("Attack2");
	}
	if (Input::GetKeyTrigger('2'))
	{
		SetAnimName2("Attack3");
	}
	if (Input::GetKeyTrigger('3'))
	{
		SetAnimName2("Attack4");
	}
	if (Input::GetKeyTrigger('4'))
	{
		SetAnimName2("Attack5");
	}
	if (Input::GetKeyTrigger('I'))
	{
		SetAnimName2("Walk");
	}
	if (Input::GetKeyTrigger('T'))
	{
		SetAnimName2("Idle");
	}
	//アニメーションの再生
	m_Model->Update(Animname1, m_Frame1, Animname2, m_Frame2, m_BlendRate);

	/*if (Input::GetController(Input::LeftUP, Input::PRESSED))
	{
		SetAnimName2("Walk");
	}*/

	//回転処理
	if (promissDirection.x * forward.x + promissDirection.y * forward.y + promissDirection.z * forward.z < 0.97 &&
		(fNormalR.x * promissDirection.x + fNormalR.z * promissDirection.z)>0) //右ベクトルとの内積
	{
		m_Rotation.y += 0.2;
	}
	else if (promissDirection.x * forward.x + promissDirection.y * forward.y + promissDirection.z * forward.z < 0.97&&
		(fNormalL.x * promissDirection.x + fNormalL.z * promissDirection.z)>0)//左ベクトルとの内積
	{
		m_Rotation.y -= 0.2;
	}

	

	switch (Pstate)
	{
	case NONE:
		//STRecover(0.75);
		break;
	case DASH:
		break;
	case ATTACK:
		break;
	case ROLLING:
		break;
	case JUMP:
		break;
	case DEATH:
		alpha -= 0.005;
		if (alpha <= 0.0)
		{
			SetDestroy();
		}
		break;
	default:
		break;
	}

	////重力
	//m_Velocity.y -= 0.015f;

	////抵抗
	//m_Velocity.y -= m_Velocity.y * 0.01f;

	////移動
	//m_Position += m_Velocity;

	//接地
	


	//// 位置が０以下なら地面位置にセットする
	//if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	//{
	//	m_Position.y = groundHeight;
	//	m_Velocity.y = 0.0f;
	//}

	// 現在シーンを取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	// ゴールとの当たり判定
	{
		Goal* goal = scene->GetGameObject<Goal>();

		if (goal)
		{
			Vector3 position = goal->GetPosition();
			Vector3 scale = goal->GetScale();

			// ゴールのAABB作成
			AABB aabbGoal;
			Vector3 GoalSize(2.0f, 2.0f, 2.0f);
			aabbGoal = SetAABB(
				position,
				fabs(GoalSize.x * scale.x),
				fabs(GoalSize.y * scale.y),
				fabs(GoalSize.z * scale.z));

			// プレイヤのAABB作成
			AABB aabbPlayer;
			Vector3 PlayerSize(1.0f, 2.0f, 1.0f);
			aabbPlayer = SetAABB(
				Vector3(m_Position.x, m_Position.y + 1.0f, m_Position.z),
				fabs(PlayerSize.x * m_Scale.x),
				fabs(PlayerSize.y * m_Scale.y),
				fabs(PlayerSize.z * m_Scale.z));

			// AABB当たり判定
			bool sts = CollisionAABB(aabbPlayer, aabbGoal);

			if (sts)
			{
				goal->SetDestroy();
			}
		}

	}

	//boxとの当たり判定(すり抜けをなくす)

	//	std::vector<Box*>box = scene->GetGameObjects<Box>();

	//	std::vector<float>Onthegap;
	//	if (box.size() != 0)
	//	{
	//		Onthegap.resize(box.size());
	//		if (box[0])
	//		{
	//			for (int i = 0; i < box.size(); i++)
	//			{
	//				Vector3 position = box[i]->GetPosition();
	//				Vector3 scale = box[i]->GetScale();
	//				Onthegap[i] = 0.0f;

	//				float a = scale.y / 3;
	//				if (a < 1)
	//				{
	//					a = 0;
	//				}

	//				// boxのAABB作成
	//				AABB aabbBox;
	//				Vector3 BoxSize(2.0f, 5.0f, 2.0f);
	//				aabbBox = SetAABB(
	//					position,
	//					fabs(BoxSize.x * scale.x),
	//					fabs(BoxSize.y * scale.y - (a * 0.1)),
	//					fabs(BoxSize.z * scale.z));

	//				// プレイヤのAABB作成
	//				AABB aabbPlayer;
	//				Vector3 PlayerSize(1.0f, 1.0f, 1.0f);
	//				aabbPlayer = SetAABB(
	//					Vector3(m_Position.x, m_Position.y + 1.0f, m_Position.z),
	//					fabs(PlayerSize.x * m_Scale.x),
	//					fabs(PlayerSize.y * m_Scale.y),
	//					fabs(PlayerSize.z * m_Scale.z));

	//				// AABB当たり判定
	//				bool stsr = CollisionAABBRight(aabbPlayer, aabbBox);
	//				bool stsl = CollisionAABBLeft(aabbPlayer, aabbBox);
	//				bool stst = CollisionAABBTop(aabbPlayer, aabbBox);
	//				bool stsb = CollisionAABBBottom(aabbPlayer, aabbBox);
	//				bool stsh = CollisionAABBHead(aabbPlayer, aabbBox);
	//				if (stsh)
	//				{
	//					//Onthegap[i] = 1.5;
	//					Onthegap[i] = scale.y * 2.0f - 0.1f;
	//					if (scale.y >= 3.0f)
	//					{
	//						Onthegap[i] = scale.y * 2.05f - 0.1f + position.y;
	//					}
	//				}
	//				/*		else if (stsr)
	//						{
	//							m_Position.x += 0.1f;
	//						}
	//						else if (stsl)
	//						{
	//							m_Position.x -= 0.1f;
	//						}
	//						else if (stst)
	//						{
	//							m_Position.z += 0.1f;
	//						}
	//						else if (stsb)
	//						{
	//							m_Position.z -= 0.1f;
	//						}*/
	//			}

	//		}
	//	}
	//float found = 0.0f;
	//for (int i = 0; i < Onthegap.size(); i++)
	//{
	//	if (Onthegap[i] > found)
	//	{
	//		found = Onthegap[i];
	//	}
	//}
	//groundHeight = found;
	//弾発射
	if (Input::GetKeyTrigger('K'))
	{
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(2);
		bullet->SetPosition(m_Position + Vector3(0.0f, 1.0f, 0.0f));
		bullet->SetVelocity(forward * 0.5f);

		m_SE->Play();
	}
	Vector3 vel = GetComponent<Rigidbody>()->GetVelocity();
	Vector3 acc = GetComponent<Rigidbody>()->GetAccel();
	Vector3 force = GetComponent<Rigidbody>()->GetForce();
	float mass = GetComponent<Rigidbody>()->GetMass();
	 //位置が０以下なら地面位置にセットする
	if (m_Position.y < groundHeight && vel.y < 0.0f)
	{
		m_Position.y = groundHeight;       //床の高さで止まる
		vel.y = 0.0f;                      //落ちる速度をリセット
		acc = force / mass;                //加速度を摩擦で減少させる
		GetComponent<Rigidbody>()->SetAccel(acc);
		GetComponent<Rigidbody>()->SetVelocity(vel);

		if (Pstate == JUMP)                //ジャンプしていればその状態をリセットする
		{
			Pstate = NONE;
		}
	}

	if (HP <= 0)
	{
		SetPstate(DEATH);
	}

	if (m_BlendRate < 1.0f)
	{
		m_BlendRate += 0.1f;
		m_Frame2+=1.0f*m_AnimSpeed;
	}
	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0;
	}
	if (m_BlendRate == 1.0f)
	{
		m_Frame1 += 1.0f * m_AnimSpeed;
	}
}

void Player::Draw()
{
	//bool B = Input::GetController(Input::b, DirectX::GamePad::ButtonStateTracker::PRESSED);
	//bool up = Input::GetController(Input::LeftUP, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool down = Input::GetController(Input::LeftDown, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool left = Input::GetController(Input::LeftLeft, DirectX::GamePad::ButtonStateTracker::HELD);
	//bool right = Input::GetController(Input::LeftRight, DirectX::GamePad::ButtonStateTracker::HELD);

	Colider* c = colattack;
	Colider* co = colme;

	ImGui::Begin("player");
	ImGui::SliderFloat("ST", &ST, 0, 100);
	ImGui::SliderFloat("HP", &HP, 0, 1000);
	ImGui::SliderFloat("Alpha", &alpha, 0, 1.0);
	ImGui::SliderFloat("posy", &m_Position.y, 0, 100.0);
	ImGui::SliderFloat("BlendRate", &m_BlendRate, 0, 1.0);
	ImGui::SliderFloat("m_Frame1", &m_Frame1, -10, 10);
	ImGui::SliderFloat("m_Frame2", &m_Frame2, -10, 10);
	ImGui::Text("Forward=x=%f,y=%f,z=%f", GetForward().x, GetForward().y, GetForward().z);
	ImGui::Text("Position=x=%f,y=%f,z=%f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::Text("State%i", Pstate);
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

}

void Player::Damage(float _damage)
{
	HP -= _damage;
}

void Player::STRecover()
{
	ST += m_RecoverST;
	if (ST >= MaxST)
	{
		ST = MaxST;
		Wait = false;
	}
}

void Player::STUse(float c)
{
	ST -= c;
	if (ST < 0)
	{
		ST = 0;
		Wait = true;
	}
}

void Player::SetVerocity(float v)
{
	m_Velocity.y = v;
}


void Player::ASHP(float hp)
{
	HP += hp;
	if (HP > MaxHP)
	{
		HP = MaxHP;
	}
	else if (HP <= 0)
	{
		SetPstate(DEATH);
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

void Player::PreDraw()
{
	//m_Model->Update("Run", m_Frame, "Idle", m_Frame, m_BlendRate);
}
