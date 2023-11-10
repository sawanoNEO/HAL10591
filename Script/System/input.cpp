#include "../System/main.h"
#include "../System/input.h"
#include "../ImGui/imguimanager.h"
using namespace DirectX;

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
GamePad Input::m_GamePad;
GamePad::State Input::m_State;
GamePad::ButtonStateTracker Input::m_GamePadButtons;
std::vector<GamePad::State> Input::m_stackState;
std::vector<GamePad::ButtonStateTracker> Input::m_stackButtons;


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

	m_State = m_GamePad.GetState(0);
	m_GamePadButtons.Update(m_State);

	//êÊçsì¸óÕÇçÏÇËÇΩÇ¢

	m_stackState.push_back(m_State);
	m_stackButtons.push_back(m_GamePadButtons);

	if (m_stackButtons.size() >= precedingInputBufferFramesLimit)
	{
		m_stackState.erase(m_stackState.begin());
		m_stackButtons.erase(m_stackButtons.begin());
		//m_stackButtons.pop_front();
	}
	/*if (m_GamePadButtons.a == m_GamePad::ButtonStateTracker::ButtonState::HELD)
	{

	}*/
	//m_GamePad.SetVibration(0, 5.0f, 5.0f, 5.0f, 5.0f);
}

void Input::Draw()
{
	ImGui::Begin("Input");
	for (auto itr = m_stackButtons.begin(), _end = m_stackButtons.end(); itr != _end; itr++)
	{
		GamePad::ButtonStateTracker tt=*itr;
		ImGui::Text("state=%i", tt.a);
	}
	ImGui::Text("GACHISTATE=%i", m_GamePadButtons.a);
	ImGui::End();
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
		if (m_GamePadButtons.a == State)
			return true;
		break;
	case Input::b:
		if (m_GamePadButtons.b == State)
			return true;
		break;
	case Input::x:
		if (m_GamePadButtons.x == State)
			return true;
		break;
	case Input::y:
		if (m_GamePadButtons.y == State)
			return true;
		break;
	case Input::L1:
		if (m_GamePadButtons.leftShoulder == State)
			return true;
		break;
	case Input::R1:
		if (m_GamePadButtons.rightShoulder == State)
			return true;
		break;
	case Input::L2:
		if (m_GamePadButtons.leftTrigger == State)
			return true;
		break;
	case Input::R2:
		if (m_GamePadButtons.rightTrigger == State)
			return true;
		break;
	case Input::LeftStick:
		if (m_GamePadButtons.leftStick == State)
			return true;
		break;
	case Input::RightStick:
		if (m_GamePadButtons.rightStick == State)
			return true;
		break;
	case Input::Start:
		if (m_GamePadButtons.start == State)
			return true;
		break;
	case Input::Back:
		if (m_GamePadButtons.back == State)
			return true;
		break;
	case Input::LeftUP:
		if (m_GamePadButtons.leftStickUp == State)
			return true;
		break;
	case Input::LeftDown:
		if (m_GamePadButtons.leftStickDown == State)
			return true;
	case Input::LeftLeft:
		if (m_GamePadButtons.leftStickLeft == State)
			return true;
	case Input::LeftRight:
		if (m_GamePadButtons.leftStickRight == State)
			return true;
	default:
		return false;
		break;
	}
	return false;
}

int Input::GetController(Button button,ButtonState State,int _bufferframe)
{
	auto itr = m_stackButtons.end();
	for (int i = 0; i < _bufferframe; i++)
	{
		itr--;
	}

	for (int i=_bufferframe;i>0;i--)
	{
		GamePad::ButtonStateTracker tempbuttonstate = *itr;
	   switch (button)
	   {
	   case Input::a:
	   	if (tempbuttonstate.a == State)
	   		return i;
	   	break;
	   case Input::b:
	   	if (tempbuttonstate.b == State)
	   		return i;
	   	break;
	   case Input::x:
	   	if (tempbuttonstate.x == State)
	   		return i;
	   	break;
	   case Input::y:
	   	if (tempbuttonstate.y == State)
	   		return i;
	   	break;
	   case Input::L1:
	   	if (tempbuttonstate.leftShoulder == State)
	   		return i;
	   	break;
	   case Input::R1:
	   	if (tempbuttonstate.rightShoulder == State)
	   		return i;
	   	break;
	   case Input::L2:
	   	if (tempbuttonstate.leftTrigger == State)
	   		return i;
	   	break;
	   case Input::R2:
	   	if (tempbuttonstate.rightTrigger == State)
	   		return i;
	   	break;
	   case Input::LeftStick:
	   	if (tempbuttonstate.leftStick == State)
	   		return i;
	   	break;
	   case Input::RightStick:
	   	if (tempbuttonstate.rightStick == State)
	   		return i;
	   	break;
	   case Input::Start:
	   	if (tempbuttonstate.start == State)
	   		return i;
	   	break;
	   case Input::Back:
	   	if (tempbuttonstate.back == State)
	   		return i;
	   	break;
	   case Input::LeftUP:
	   	if (tempbuttonstate.leftStickUp == State)
	   		return i;
	   	break;
	   case Input::LeftDown:
	   	if (tempbuttonstate.leftStickDown == State)
	   		return i;
	   case Input::LeftLeft:
	   	if (tempbuttonstate.leftStickLeft == State)
	   		return i;
	   case Input::LeftRight:
	   	if (tempbuttonstate.leftStickRight == State)
	   		return i;
	   	break;
	   }
	   itr++;
	}
	//éwíËÇÃì¸óÕÇ™å©Ç¬Ç©ÇÁÇ»Ç©Ç¡ÇΩèÍçáé∏îsÇÃílÇï‘Ç∑
	return -1;
}

float Input::GetStick(DIR dir)
{
	switch (dir)
	{
	case Input::LeftX:
		return m_State.thumbSticks.leftX;
		break;
	case Input::LeftY:
		return m_State.thumbSticks.leftY;
		break;
	case Input::RightX:
		return m_State.thumbSticks.rightX;
		break;
	case Input::RightY:
		return m_State.thumbSticks.rightY;
		break;
	default:
		break;
	}
	return 0.0f;
}


