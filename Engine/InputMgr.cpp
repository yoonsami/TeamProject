#include "pch.h"
#include "InputMgr.h"

int g_arrVK[static_cast<int>(KEY_TYPE::LAST)] =
{
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	'W',	'A',	'S',	'D',	'Q',	'E',	'Z',	'C',	'R', 'X','V',
	'1',	'2',	'3',	'4',	'5',
	VK_F1, VK_F2, VK_F3, VK_F4,


	VK_SPACE,
	VK_ESCAPE,
	VK_RETURN,
	VK_LCONTROL,
	VK_LSHIFT,
	VK_MENU,
	VK_LBUTTON,
	VK_RBUTTON
};


void InputMgr::Initialize(HWND hWnd)
{
	m_vecKeyStates.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
	m_hWnd = hWnd;
}

void InputMgr::Tick()
{
	HWND hWnd = ::GetActiveWindow();
	if (m_hWnd != hWnd)
	{
		for (auto& i : m_vecKeyStates)
			i = KEY_STATE::NONE;
		m_vMouseDir = { 0.f,0.f };
		return;
	}

	for (_uint key = 0; key < KEY_TYPE_COUNT; ++key)
	{
		if (GetAsyncKeyState(g_arrVK[key]) & 0x8000)
		{
			KEY_STATE& state = m_vecKeyStates[key];

			if (state == KEY_STATE::HOLD || state == KEY_STATE::TAP)
			{
				state = KEY_STATE::HOLD;
			}
			else
			{
				state = KEY_STATE::TAP;
			}
		}
		else
		{
			KEY_STATE& state = m_vecKeyStates[key];

			if (state == KEY_STATE::HOLD || state == KEY_STATE::TAP)
			{
				state = KEY_STATE::AWAY;
			}
			else
			{
				state = KEY_STATE::NONE;
			}
		}
	}

	POINT MousPos;

	::GetCursorPos(&MousPos);
	::ScreenToClient(m_hWnd, &MousPos);

	m_ptMousePos = { _float(MousPos.x),_float(MousPos.y) };

	if (GetButtonTap(KEY_TYPE::LALT))
	{
		RECT tmp{};
		GetClientRect(hWnd, &tmp);

		POINT center = { (tmp.right - tmp.left) / 2, (tmp.bottom - tmp.top) / 2 };

		ClientToScreen(hWnd, &center);
		SetCursorPos(center.x, center.y);
	}
	else if(GetButtonHold(KEY_TYPE::LALT) == true)
	{
		RECT tmp{};
		GetClientRect(hWnd, &tmp);

		POINT center = { (tmp.right - tmp.left) / 2, (tmp.bottom - tmp.top) / 2 };

		m_vMouseDir.x = _float(m_ptMousePos.x - center.x);
		m_vMouseDir.y = _float(m_ptMousePos.y - center.y);
		ClientToScreen(hWnd, &center);
		//::ShowCursor(false);
		SetCursorPos(center.x, center.y);
	}
	else
	{
		m_vMouseDir = { 0.f,0.f };
		::ShowCursor(true);

	}

}