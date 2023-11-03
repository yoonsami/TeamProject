#pragma once

struct GRAPHICDESC
{
	enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

	HWND				hWnd;
	unsigned int		iSizeX, iSizeY;
	WINMODE				eWinMode;
	DirectX::XMFLOAT4	vClearColor;

};