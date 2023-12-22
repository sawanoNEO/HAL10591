#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>
#include <vector>
#include "Item.h"

class StateItem :  public State
{
private:
	std::vector<class Item*> m_Items;//�����Ă���A�C�e��
	int m_Cursol;//�g�p�����A�C�e���̔ԍ�
public:
	using State::State;
	void Init(ITEMID);//�I�[�o�[���C�h���Ă��Ȃ��̂ŌĂ�ł�����K�v������
	void Uninit()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

