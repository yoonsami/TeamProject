#pragma once

#ifdef _DEBUGTOOL
class DebugTool
{
	DECLARE_SINGLETON(DebugTool)

public:
	HRESULT Init();
	void Tick();
	void Render();

public:
	void RenderOptionTap();
};

#endif