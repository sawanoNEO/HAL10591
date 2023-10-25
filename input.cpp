#include "main.h"
#include "input.h"
#include "imguimanager.h"
using namespace DirectX;

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
GamePad Input::gamePad;
GamePad::State Input::state;
GamePad::ButtonStateTracker Input::gamePadButtons;

void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void Input::Uninit()
{


}

void Input::Update()
{
    
	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

	state = gamePad.GetState(0);
	gamePadButtons.Update(state);
	/*if (gamePadButtons.a == GamePad::ButtonStateTracker::ButtonState::HELD)
	{

	}*/
	//gamePad.SetVibration(0, 5.0f, 5.0f, 5.0f, 5.0f);
}



bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetStickState()
{
	if (Input::GetStick(LeftX) < 0.2 &&
		Input::GetStick(LeftX) > -0.2 &&
		Input::GetStick(LeftY) < 0.2 &&
		Input::GetStick(LeftY) > -0.2)
	{
		return false;
	}
	return true;
}

bool Input::GetController(Button button,ButtonState State)
{
	switch (button)
	{
	case Input::a:
		if (gamePadButtons.a == State)
			return true;
		break;
	case Input::b:
		if (gamePadButtons.b == State)
			return true;
		break;
	case Input::x:
		if (gamePadButtons.x == State)
			return true;
		break;
	case Input::y:
		if (gamePadButtons.y == State)
			return true;
		break;
	case Input::L1:
		if (gamePadButtons.leftShoulder == State)
			return true;
		break;
	case Input::R1:
		if (gamePadButtons.rightShoulder == State)
			return true;
		break;
	case Input::L2:
		if (gamePadButtons.leftTrigger == State)
			return true;
		break;
	case Input::R2:
		if (gamePadButtons.rightTrigger == State)
			return true;
		break;
	case Input::LeftStick:
		if (gamePadButtons.leftStick == State)
			return true;
		break;
	case Input::RightStick:
		if (gamePadButtons.rightStick == State)
			return true;
		break;
	case Input::Start:
		if (gamePadButtons.start == State)
			return true;
		break;
	case Input::Back:
		if (gamePadButtons.back == State)
			return true;
		break;
	case Input::LeftUP:
		if (gamePadButtons.leftStickUp == State)
			return true;
		break;
	case Input::LeftDown:
		if (gamePadButtons.leftStickDown == State)
			return true;
	case Input::LeftLeft:
		if (gamePadButtons.leftStickLeft == State)
			return true;
	case Input::LeftRight:
		if (gamePadButtons.leftStickRight == State)
			return true;
	default:
		return false;
		break;
	}
	return false;
}

float Input::GetStick(DIR dir)
{
	switch (dir)
	{
	case Input::LeftX:
		return state.thumbSticks.leftX;
		break;
	case Input::LeftY:
		return state.thumbSticks.leftY;
		break;
	case Input::RightX:
		return state.thumbSticks.rightX;
		break;
	case Input::RightY:
		return state.thumbSticks.rightY;
		break;
	default:
		break;
	}
	return 0.0f;
}


