#pragma once
#include "../StateMachine.h"


class EStateChase : public State
{
private:
	enum Action
	{
		NONE,
		ATTACK,
		WAITANDSEE,
	};
	int m_AttackRate = 70;//���̍s���𔻒肷��ۂɁA���̍s�����U���ɂȂ�m��(%)
	int m_WaitandSeeRate = 30;//���̍s���𔻒肷��ۂɁA���̍s�����l�q���ɂȂ�m��(%)
	int m_SpecialAttack = 0;//���̍s���𔻒肷��ۂɁA���̍s��������U���ɂȂ�m��(%)
	int m_RecognitionLength = 0;
	Action m_Action;

public:
	State::State;
	void Init(int _attackrate, int _waitandseerate,int _specialAttack);
	void Init(const char* FilePath);
	void Enter()override;
	void Exit()override;
	void StateUpdate()override;
	void StateChange()override;
	void Draw()override;
};

