#pragma once

enum class KEY_TYPE
{
	UP,
	DOWN,
	LEFT,
	RIGHT,

	W,	A,	S,	D,	Q,	E,	Z,	C,	R,
	KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,

	SPACE,
	ESC,
	ENTER,
	LCTRL,
	LSHIFT,
	LALT,
	LBUTTON,
	RBUTTON,
	LAST
};


enum class KEY_STATE
{
	NONE,
	HOLD,
	TAP,
	AWAY,
	KEYSTATE_END
};

enum
{
	KEY_TYPE_COUNT = static_cast<_uint>(VK_MAX + 1),
	KEY_STATE_COUNT = static_cast<_uint>(KEY_STATE::KEYSTATE_END)

};


class InputMgr 
{
	DECLARE_SINGLETON(InputMgr)

public:
	void Initialize(HWND hWnd);
	void Tick();

public:
	bool GetButtonHold(KEY_TYPE key) { return GetState(key) == KEY_STATE::HOLD; }
	bool GetButtonTap(KEY_TYPE key) { return GetState(key) == KEY_STATE::TAP; }
	bool GetButtonAway(KEY_TYPE key) { return GetState(key) == KEY_STATE::AWAY; }

	const _float2& GetMousePos() { return m_ptMousePos; }
	const _float2& GetMouseDir() { return m_vMouseDir; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return m_vecKeyStates[static_cast<_uchar>(key)]; }

private:
	HWND				m_hWnd = nullptr;
	vector<KEY_STATE>	m_vecKeyStates;
	_float2				m_ptMousePos{};

	_float2				m_vMouseDir{};
};

