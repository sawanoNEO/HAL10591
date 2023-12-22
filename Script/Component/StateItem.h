#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>
#include <vector>
#include "Item.h"

class StateItem :  public State
{
private:
	std::vector<class Item*> m_Items;//持っているアイテム
	int m_Cursol;//使用されるアイテムの番号
public:
	using State::State;
	void Init(ITEMID);//オーバーライドしていないので呼んであげる必要がある
	void Uninit()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

