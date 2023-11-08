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
	void RenderOptionTab();
	void FogOptionTab();
	void LensFlareTab();

private:
	_bool m_bIsRender = true;
};

#endif