#pragma once

#ifdef _DEBUGTOOL
class DebugTool
{
	DECLARE_SINGLETON(DebugTool)
public:
	~DebugTool();

public:
	HRESULT Init();
	void Tick();
	void Render();

public:
	void RenderOptionTap();
	void FogOptionTap();

private:
	_bool m_bIsRender = true;
};

#endif