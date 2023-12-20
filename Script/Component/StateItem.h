#pragma once
#include "StateMachine.h"
#include <SimpleMath.h>

class StateItem :  public State
{
private:
	class Item* m_CurrentItem;
public:
	using State::State;
	void Init(Item* _item);//オーバーライドしていないので呼んであげる必要がある
	void Uninit()override;
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

