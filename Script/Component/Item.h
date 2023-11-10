#pragma once
#include "../Component/component.h"
class Item :  public Component
{
private:
	int Wait;   //アイテム使用までの前隙
	int Recovery;   //アイテム使用後の後隙
public:
	using Component :: Component;
	void SetWait(int wait) { Wait = wait; }
	void SetRecovery(int recover) { Recovery = recover; }
	int GetWait() { return Wait; }
	int GetRecovery() { return Recovery; }
};

