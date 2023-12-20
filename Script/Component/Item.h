#pragma once
#include "component.h"

class Item:public Component
{
protected:
	int m_Count = 0;
	int m_InventoryCount;
	int m_MaxInventoryCount;
	int m_StartUpTime;
	int m_ActionTime;
	int m_RecoveryTime;
	bool m_ChangeStateFlg=false;
	const char* AnimName;

public:
	using Component::Component;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
	virtual void Draw() = 0;

	const char* GetAnimName() { return AnimName; }
	bool GetChangeFlg() { return m_ChangeStateFlg; }
};

