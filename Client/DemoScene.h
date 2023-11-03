#pragma once
#include "Scene.h"
class DemoScene :
    public Scene
{
public:
	DemoScene();
	~DemoScene();

public:
	virtual void Init() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Final_Tick() override;

	virtual HRESULT Load_Scene() override;

private:
	void Load_Spear_Ace();
	void Load_Kyle();
	void Load_DemoModel();
	void Load_DemoMap();
	void Load_Camera();
	void Load_Light();
};

