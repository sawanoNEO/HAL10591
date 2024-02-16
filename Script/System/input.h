#pragma once
#include <windows.h>
#include <GamePad.h>
#include <vector>

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static DirectX::GamePad m_GamePad;
	static DirectX::GamePad::State m_State;
	static DirectX::GamePad::ButtonStateTracker m_GamePadButtons;
	static std::vector<DirectX::GamePad::State> m_stackState;
	static std::vector<DirectX::GamePad::ButtonStateTracker> m_stackButtons;
	static const int precedingInputBufferFramesLimit = 60;  //��s���͂��󂯕t����P�\�t���[���̏��(���̒l�̕������p�b�h���͂��X�^�b�N����)

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	enum Button  //�{�^���̎�ށB�����Ɏg�p�BLeftUp���͍��X�e�B�b�N�̌X���̏�Ԃ��擾����
	{
		a,b,x,y,L1,R1,L2,R2,LeftStick,RightStick,Start,Back,LeftUP,LeftDown,LeftLeft,LeftRight,CROSSUP,CROSSDOWN,CROSSLEFT,CROSSRIGHT
	};
	enum DIR   //�e�X�e�B�b�N�̌X�����擾���邽�߂̒l�B
	{
		LeftX,LeftY,RightX,RightY,
	};
	enum ButtonState   //�e�{�^���̉�����Ԃ̎��
	{
		UP = 0,         // Button is up ������Ă��Ȃ�
		HELD = 1,       // Button is held down �������ςȂ�
		RELEASED = 2,   // Button was just released �{�^���������ꂽ�u��
		PRESSED = 3,    // Buton was just pressed �{�^���������ꂽ�u��
	};

	static bool GetStickState();//���X�e�B�b�N�ɂ�鑀�삪�s���Ă��邩�ǂ����𔻒肷��
	static bool GetController(Button,ButtonState);    //�R���g���[���[�̃{�^���̉�����Ԃ��擾����֐�(��s���͂��󂯕t���Ȃ�)
	static int GetController(Button,ButtonState,int _bufferframe);    //�R���g���[���[�̃{�^���̉�����Ԃ��擾����֐�(��s���͂��󂯕t����)�B�Ԃ�l�͂ǂꂾ���O�̃t���[���ł��̓��͂������������B(-1�͎��s)
	static float GetStick(DIR);                       //�X�e�B�b�N�̌X������擾����֐��@

};
