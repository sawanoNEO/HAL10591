#pragma once
#include <windows.h>
#include <GamePad.h>
#include <list>

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static DirectX::GamePad m_GamePad;
	static DirectX::GamePad::State m_State;
	static DirectX::GamePad::ButtonStateTracker m_GamePadButtons;
	static std::list<DirectX::GamePad::ButtonStateTracker> m_stackButtons;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	enum Button  //ボタンの種類。引数に使用。LeftUp等は左スティックの傾きの状態を取得する
	{
		a,b,x,y,L1,R1,L2,R2,LeftStick,RightStick,Start,Back,LeftUP,LeftDown,LeftLeft,LeftRight
	};
	enum DIR   //各スティックの傾きを取得するための値。
	{
		LeftX,LeftY,RightX,RightY,
	};
	enum ButtonState   //各ボタンの押下状態の種類
	{
		UP = 0,         // Button is up 押されていない
		HELD = 1,       // Button is held down 押しっぱなし
		RELEASED = 2,   // Button was just released ボタンが離された瞬間
		PRESSED = 3,    // Buton was just pressed ボタンが押された瞬間
	};

	static bool GetStickState();//左スティックによる操作が行われているかどうかを判定する
	static bool GetController(Button,ButtonState);    //コントローラーのボタンの押下状態を取得する関数
	static float GetStick(DIR);                       //スティックの傾き具合を取得する関数　

};
