#pragma once
class MainApp
{
public:
	MainApp();
	virtual ~MainApp() = default;

public:
	HRESULT Init();
	void	Tick();
	HRESULT Render();

private:
	HRESULT Open_Scene();
};

