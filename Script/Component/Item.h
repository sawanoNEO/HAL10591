#pragma once
#include "component.h"

enum ITEMID
{
	HEAL,
};

class Item
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
	class GameObject* m_GameObject;

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
	virtual void Draw() = 0;
	void SetGameObject(GameObject* _gameobject) { m_GameObject = _gameobject; }

	const char* GetAnimName() { return AnimName; }
	bool GetChangeFlg() { return m_ChangeStateFlg; }
};

