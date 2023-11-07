#pragma once
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

private:
	_bool m_bIsRender;
};

