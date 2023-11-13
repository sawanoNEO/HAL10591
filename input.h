#pragma once
#include <windows.h>
#include <GamePad.h>
<<<<<<< Updated upstream:input.h

=======
#include <list>
>>>>>>> Stashed changes:Script/System/input.h

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
<<<<<<< Updated upstream:input.h
	static DirectX::GamePad gamePad;
	static DirectX::GamePad::State state;
	static DirectX::GamePad::ButtonStateTracker gamePadButtons;
=======
	static DirectX::GamePad m_GamePad;
	static DirectX::GamePad::State m_State;
	static DirectX::GamePad::ButtonStateTracker m_GamePadButtons;

	//////////////����list�ɕς��܂��傤(2023/11/11)
	static std::list<DirectX::GamePad::State> m_stackState;
	static std::list<DirectX::GamePad::ButtonStateTracker> m_stackButtons;
	static const int precedingInputBufferFramesLimit = 60;  //��s���͂��󂯕t����P�\�t���[���̏��(���̒l�̕������p�b�h���͂��X�^�b�N����)
>>>>>>> Stashed changes:Script/System/input.h

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	enum Button  //�{�^���̎�ށB�����Ɏg�p�BLeftUp���͍��X�e�B�b�N�̌X���̏�Ԃ��擾����
	{
		a,b,x,y,L1,R1,L2,R2,LeftStick,RightStick,Start,Back,LeftUP,LeftDown,LeftLeft,LeftRight
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
	static bool GetController(Button,ButtonState);    //�R���g���[���[�̃{�^���̉�����Ԃ��擾����֐�
	static float GetStick(DIR);                       //�X�e�B�b�N�̌X������擾����֐��@
	static void EditBufferFrame(const char* ActionName, Button, ButtonState,int _bufferframe);//��s���̗͂P�\�t���[����ݒ肷��֐��ADraw�֐��ł̂݋@�\�B
};
