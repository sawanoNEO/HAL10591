#pragma once

#include	<SimpleMath.h>
#include	"gameObject.h"

class Colider;//�O���錾

class Camera : public GameObject{
private:

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};
	DirectX::SimpleMath::Vector3    m_GoalPosition{};//�ڕW�n�_
	float angle;
	float Yangle;      //Y���v�Z�p�̕ϐ�
	DirectX::SimpleMath::Vector3    camForward;
	float Distance=13.0f;//�J�����ƃv���C���[�Ԃ̋���
	bool rock;
	GameObject* RockEnemy;
	Colider* camcol;    //�J�����̕ǉ���̂��߂ɓ����蔻���t����

	float TargetY;//�^�[�Q�b�g���̃J�����̍���(�����p)

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Begin();
	DirectX::SimpleMath::Vector3 GetCamForward();   //�J�����̌����Ă���������擾
	DirectX::SimpleMath::Vector3 GetCamSide();      //���̉��̕������擾����ver
	DirectX::SimpleMath::Vector3 VecYRemove(DirectX::SimpleMath::Vector3);      //�J�����̑O�����x�N�g����Y��������菜������(�ړ��Ɏg��)
	DirectX::SimpleMath::Matrix GetViewMatrix() 
	{ 
		return m_ViewMatrix;
	}
	bool GetRock() { return rock; }
	GameObject* GetRockEnemy() { return RockEnemy; }
};