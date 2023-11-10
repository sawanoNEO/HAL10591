#pragma once
#include "../Component/component.h"
class Item :  public Component
{
private:
	int Wait;   //�A�C�e���g�p�܂ł̑O��
	int Recovery;   //�A�C�e���g�p��̌㌄
public:
	using Component :: Component;
	void SetWait(int wait) { Wait = wait; }
	void SetRecovery(int recover) { Recovery = recover; }
	int GetWait() { return Wait; }
	int GetRecovery() { return Recovery; }
};

