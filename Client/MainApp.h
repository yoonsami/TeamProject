#pragma once
class MainApp
{
public:
	MainApp();
	virtual ~MainApp();

public:
	HRESULT Init();
	void	Tick();
	HRESULT Render();

	static int tickTime;
	static int renderTime;

private:
	HRESULT Open_Scene();

	void Load_Ui();
};

