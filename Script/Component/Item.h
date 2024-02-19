#pragma once
#include "component.h"

enum ITEMID//アイテムの種類
{
	HEAL,
};

class Item
{
protected:
	int m_Count = 0;//アイテム使用動作が何f続いているか
	int m_InventoryCount;//アイテム所持数
	int m_MaxInventoryCount;//アイテム最大所持数
	int m_StartUpTime;//効果が発揮されるまでのフレーム
	int m_ActionTime;//効果が発揮されているフレーム
	int m_RecoveryTime;//アイテム使用後の硬直
	bool m_ChangeStateFlg=false;//状態の切り替えを判断
	const char* m_AnimName;//各アイテムに連動しているアニメーションの名前
	class GameObject* m_GameObject;

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
	virtual void Draw() = 0;
	void SetGameObject(GameObject* _gameobject) { m_GameObject = _gameobject; }

	const char* GetAnimName() { return m_AnimName; }
	bool GetChangeFlg() { return m_ChangeStateFlg; }
};

