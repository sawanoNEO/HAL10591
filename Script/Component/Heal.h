#pragma once
#include "Item.h"
class Heal : public Item
{
private:
	int m_ParticleNum=1;
	//DirectX::SimpleMath::Vector3 m_SpawnPosition{1.0f,1.0f,1.0f};//�p�[�e�B�N����gameobject����ǂꂾ�����ꂽ�ʒu�ɏo�����͈̔�(�����͈̔�)
public:
	void Enter()override;
	void Exit()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};

