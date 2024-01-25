#pragma once

#include "../System/renderer.h"

#include <list>
#include <SimpleMath.h>

#include "../Component/component.h"


class GameObject
{

protected:
	DirectX::SimpleMath::Matrix m_Matrix;
	bool		m_Destroy = false;

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
	DirectX::SimpleMath::Vector3	m_OldPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	std::list<Component*> m_Component;

	std::list<GameObject*> m_ChildGameObject;

public:
	GameObject() {}//�R���X�g���N�^
	virtual ~GameObject() {}//�f�X�g���N�^�i���z�֐��j


	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }
	void SetPosition(DirectX::SimpleMath::Vector3 Position) { m_Position = Position; }
	void SetRotation(DirectX::SimpleMath::Vector3 Rotation) { m_Rotation = Rotation; }
	void SetScale(DirectX::SimpleMath::Vector3 Scale) { m_Scale = Scale; }
	virtual void SetAnimName2(const char*) {};//�A�j���[�V�����̋@�\������I�u�W�F�N�g�́A���̊֐����g�����ƂōĐ�����A�j���[�V������؂�ւ��鎖���o����
	virtual void SetFrame1(int) {};//�A�j���[�V�����t���[��1���Z�b�g
	virtual void SetFrame2(int) {};//�A�j���[�V�����t���[��2���Z�b�g
	virtual void Damage(float _damage) {};//�_���[�W���󂯂鎞�̏���

	DirectX::SimpleMath::Matrix GetMatrix() { return m_Matrix; }

	DirectX::SimpleMath::Vector3 GetForward()//�O�����x�N�g���擾
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	
	DirectX::SimpleMath::Vector3 GetSide()//�������x�N�g���擾
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y+89.535f, m_Rotation.x, m_Rotation.z);//89.535�̓}�W�b�N�i���o�[

		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	DirectX::SimpleMath::Vector3 GetOldPosition()//�O��̃t���[���̃|�W�V������Ԃ�
	{
		return m_OldPosition;
	}

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			UninitBase();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void PreDraw() {}

	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();

		return component;
	}

	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();

		return child;
	}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		Uninit();
		// ts 20230630�i�q���Q�[���I�u�W�F�N�g�̏I�������Y���ǉ��j
		for (auto* childgo : m_ChildGameObject)
		{
			childgo->Uninit();

			for (Component* component : childgo->m_Component)
			{
				component->Uninit();
				delete component;
			}
			childgo->m_Component.clear();//���X�g�̃N���A

			delete childgo;
		}
		m_ChildGameObject.clear();//���X�g�̃N���A

		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();//���X�g�̃N���A
	}

	void UpdateBase()
	{
		for (GameObject* child : m_ChildGameObject)
		{
			child->UpdateBase();
		}

		for (Component* component : m_Component)
		{
			component->Update();
		}

		Update();
		m_OldPosition = m_Position;
	}

	void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		Draw();
		PreDraw();
		// �}�g���N�X�ݒ�
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans * ParentMatrix;

		m_Matrix = world;

		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawBase(world);
		}


		Renderer::SetWorldMatrix(&world);

		for (Component* component : m_Component)
		{
			component->Draw();
		}

	}
};